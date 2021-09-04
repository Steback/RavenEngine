#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "math/Common.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


namespace re {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName, std::shared_ptr<AssetsManager> assetsManager)
            : device(std::move(device)), assetsManager(std::move(assetsManager)) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstant);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert", shadersName + ".frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{this->assetsManager->getDescriptorSetLayout()},
                std::vector<VkPushConstantRange>{pushConstantRange}
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer) {
        if (camera) {
            auto& cameraComponent = camera->getComponent<Camera>();
            mat4 viewProj = cameraComponent.getProjection() * cameraComponent.getView();

            pipeline->bind(commandBuffer);
            for (auto& id : scene->getRegistry().view<Transform, MeshRender>()) {
                auto entity = scene->getEntity(id);
                auto& transform = entity->getComponent<Transform>();
                auto& meshRender = entity->getComponent<MeshRender>();

                PushConstant push;
                push.mvp = viewProj * transform.getWorldMatrix();

                meshRender.model->render(commandBuffer, pipeline->getLayout(), push);
            }
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

    void RenderSystem::setScene(std::shared_ptr<Scene> scene_) {
        scene = std::move(scene_);
        camera = scene->getEntity("Camera");
    }

} // namespace re