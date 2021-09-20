#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "scene/Skybox.hpp"
#include "math/Common.hpp"
#include "assets/AssetsManager.hpp"
#include "assets/Material.hpp"
#include "entity/Entity.hpp"
#include "render/buffers/UniformBuffer.hpp"


namespace re {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName)
            : device(std::move(device)) {
        VkPushConstantRange materialPushConstant{};
        materialPushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        materialPushConstant.size = sizeof(Material::PushConstantBlock);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);

        setupBuffer();
        setupDescriptors();

        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert", shadersName + ".frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{AssetsManager::getInstance()->getDescriptorSetLayout(UBO), AssetsManager::getInstance()->getDescriptorSetLayout(MATERIAL)},
                std::vector<VkPushConstantRange>{materialPushConstant}
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer, const std::shared_ptr<Scene>& scene) {
        if (!camera) camera = scene->getEntity("Camera");
        if (!light) light = scene->getEntity("Light");

        auto& cameraComponent = camera->getComponent<Camera>();
        mat4 viewProj = cameraComponent.getProjection() * cameraComponent.getView();

        if (scene->skybox)
            scene->skybox->draw(commandBuffer, cameraComponent.getProjection(), Matrix4{1.0f});

        pipeline->bind(commandBuffer);

        if (light) {
            auto& lightComponent = light->getComponent<Light>();
            auto& transform = light->getComponent<Transform>();
            uboLight.position = transform.position;
            uboLight.color = lightComponent.color;
            uboLight.ambient = lightComponent.ambient;
            uboLightBuffer->update(&uboLight);
        }

        std::vector<VkDescriptorSet> sets = {uboDescriptorSet};
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->getLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                0, nullptr);

        for (auto& id : scene->getRegistry().view<Transform, MeshRender>()) {
            auto entity = scene->getEntity(id);
            auto& transform = entity->getComponent<Transform>();
            auto& meshRender = entity->getComponent<MeshRender>();

            mat4 transformMatrix = transform.getWorldMatrix();
            uboTransform.mvp = viewProj * transformMatrix;

            meshRender.model->render(commandBuffer, pipeline->getLayout(), uboNode);
            updateBuffer();
        }
    }

    std::shared_ptr<Entity> RenderSystem::getCamera() {
        return camera;
    }

    void RenderSystem::update(float aspect) {
        if (camera) {
            auto& cameraComponent = camera->getComponent<Camera>();
            cameraComponent.setPerspectiveProjection(radians(50.f), aspect, 0.1f, 10.f);
        }
    }

    // TODO: Remove this descriptor pool and layout from here
    void RenderSystem::setupDescriptors() {
        AssetsManager::getInstance()->allocateDescriptorSet(DescriptorSetType::UBO, &uboDescriptorSet);

        std::array<VkWriteDescriptorSet, 3> writeDescriptorSets{};
        writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSets[0].descriptorCount = 1;
        writeDescriptorSets[0].dstSet = uboDescriptorSet;
        writeDescriptorSets[0].dstBinding = 0;
        VkDescriptorBufferInfo transformDescriptor = uboTransformBuffer->getDescriptor();
        writeDescriptorSets[0].pBufferInfo = &transformDescriptor;

        writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSets[1].descriptorCount = 1;
        writeDescriptorSets[1].dstSet = uboDescriptorSet;
        writeDescriptorSets[1].dstBinding = 1;
        VkDescriptorBufferInfo nodeDescriptor = uboNodeBuffer->getDescriptor();
        writeDescriptorSets[1].pBufferInfo = &nodeDescriptor;

        writeDescriptorSets[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSets[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSets[2].descriptorCount = 1;
        writeDescriptorSets[2].dstSet = uboDescriptorSet;
        writeDescriptorSets[2].dstBinding = 2;
        VkDescriptorBufferInfo lightDescriptor = uboLightBuffer->getDescriptor();
        writeDescriptorSets[2].pBufferInfo = &lightDescriptor;

        vkUpdateDescriptorSets(device->getDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
    }

    // TODO: Change how uniform buffer are used by RenderSystem
    void RenderSystem::setupBuffer() {
        uboTransformBuffer = std::make_unique<UniformBuffer>(
                device->getAllocator(),
                sizeof(Transform::Ubo)
        );
        uboNodeBuffer = std::make_unique<UniformBuffer>(
                device->getAllocator(),
                sizeof(Model::Ubo)
        );
        uboLightBuffer = std::make_unique<UniformBuffer>(
                device->getAllocator(),
                sizeof(Light::Ubo)
        );
    }

    // TODO: Remove this
    void RenderSystem::updateBuffer() {
        uboTransformBuffer->update(&uboTransform);
        uboNodeBuffer->update(&uboNode);
    }

} // namespace re