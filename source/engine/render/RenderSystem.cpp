#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "engine/render/pipelines/GraphicsPipeline.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Skybox.hpp"
#include "engine/math/Math.hpp"
#include "engine/assets/AssetsManager.hpp"
#include "engine/assets/Material.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/render/buffers/UniformBuffer.hpp"


namespace re {

    // TODO: Refactored RenderSystem class and add doxygen comments
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
                std::vector<VkPushConstantRange>{materialPushConstant}
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer, const std::shared_ptr<Scene>& scene) {
        if (!camera) camera = scene->getMainCamera();
        if (!light) light = scene->getEntity("Light");

        auto& cameraComponent = camera->getComponent<Camera>();
        mat4 viewProj = cameraComponent.projection * cameraComponent.view;

        if (scene->skybox)
            scene->skybox->draw(commandBuffer, cameraComponent.projection, Matrix4{1.0f});

        pipeline->bind(commandBuffer);

        if (light) {
            auto& lightComponent = light->getComponent<Light>();
            auto& transform = light->getComponent<Transform>();
            uboLight.position = transform.position;
            uboLight.color = lightComponent.color;
            uboLight.ambient = lightComponent.ambient;
            uboLight.viewPosition = camera->getComponent<Transform>().position;
            uboLightBuffer->writeTo(&uboLight);
        }

        std::vector<VkDescriptorSet> sets = {uboDescriptorSet};
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->getLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                0, nullptr);

        for (auto& id : scene->getRegistry().view<Transform, MeshRender>()) {
            auto entity = scene->getEntity(id);
            auto& meshRender = entity->getComponent<MeshRender>();

            if (meshRender.enable) {
                auto& transform = entity->getComponent<Transform>();

                mat4 transformMatrix = transform.worldMatrix();
                uboTransform.mvp = viewProj * transformMatrix;
                uboTransform.invTransform = transformMatrix.inverted();

                meshRender.model->render(commandBuffer, pipeline->getLayout(), uboNode);
                updateBuffer();
            }
        }
    }

    std::shared_ptr<Entity> RenderSystem::getCamera() {
        return camera;
    }

    void RenderSystem::update(float aspect) {
        if (camera) {
            auto& cameraComponent = camera->getComponent<Camera>();
            cameraComponent.setPerspective(aspect);
        }
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
        uboTransformBuffer->writeTo(&uboTransform);
        uboNodeBuffer->writeTo(&uboNode);
    }

} // namespace re