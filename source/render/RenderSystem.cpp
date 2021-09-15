#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "scene/Skybox.hpp"
#include "math/Common.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/Entity.hpp"
#include "render/Buffer.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


namespace re {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName)
            : device(std::move(device)) {
        VkPushConstantRange materialPushConstant{};
        materialPushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        materialPushConstant.size = sizeof(PushConstBlockMaterial);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);

        setupBuffer();
        setupDescriptors();

        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert", shadersName + ".frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{AssetsManager::getInstance()->getUboLayout(), AssetsManager::getInstance()->getMaterialLayout()},
                std::vector<VkPushConstantRange>{materialPushConstant}
        );
    }

    RenderSystem::~RenderSystem() {
        uboModelBuffer->unmap();
    }

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer, const std::shared_ptr<Scene>& scene) {
        if (!camera) camera = scene->getEntity("Camera");

        auto& cameraComponent = camera->getComponent<Camera>();
        mat4 viewProj = cameraComponent.getProjection() * cameraComponent.getView();

        scene->skybox->draw(commandBuffer, cameraComponent.getProjection(), Matrix4{1.0f});

        pipeline->bind(commandBuffer);

        std::vector<VkDescriptorSet> sets = {uboDescriptorSet};
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->getLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                0, nullptr);

        for (auto& id : scene->getRegistry().view<Transform, MeshRender>()) {
            auto entity = scene->getEntity(id);
            auto& transform = entity->getComponent<Transform>();
            auto& meshRender = entity->getComponent<MeshRender>();

            uboModel.mvp = viewProj * transform.getWorldMatrix();

            meshRender.model->render(commandBuffer, pipeline->getLayout(), uboModel);
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

        VkWriteDescriptorSet writeDescriptorSet{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.dstSet = uboDescriptorSet;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.pBufferInfo = &uboModelBuffer->descriptor;
        vkUpdateDescriptorSets(device->getDevice(), 1, &writeDescriptorSet, 0, nullptr);
    }

    void RenderSystem::setupBuffer() {
        uboModelBuffer = std::make_unique<Buffer>(
                device->getAllocator(),
                sizeof(UboModel),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_MEMORY_USAGE_CPU_TO_GPU
        );
        uboModelBuffer->map();
    }

    // TODO: Remove this
    void RenderSystem::updateBuffer() {
        uboModelBuffer->copyTo(&uboModel);
    }

} // namespace re