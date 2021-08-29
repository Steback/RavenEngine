#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "math/Common.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


namespace re {

    struct MvpPushConstant {
        mat4 mvp{1.f};
    };

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName, Scene& scene)
            : device(std::move(device)), scene(scene) {
        camera = scene.getEntity("Camera");

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(MvpPushConstant);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert.spv", shadersName + ".frag.spv",
                configInfo,
                std::vector<VkPushConstantRange>{pushConstantRange}
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer) {
        auto& cameraComponent = camera->getComponent<Camera>();
        mat4 viewProj = cameraComponent.getProjection() * cameraComponent.getView();

        pipeline->bind(commandBuffer);
        for (auto& id : scene.getRegistry().view<Transform, MeshRender>()) {
            auto entity = scene.getEntity(id);
            auto& transform = entity->getComponent<Transform>();
            auto& meshRender = entity->getComponent<MeshRender>();

            MvpPushConstant pushMvp;
            pushMvp.mvp = viewProj * transform.getWorldMatrix();

            vkCmdPushConstants(
                    commandBuffer,
                    pipeline->getLayout(),
                    VK_SHADER_STAGE_VERTEX_BIT,
                    0,
                    sizeof(MvpPushConstant),
                    &pushMvp);

            meshRender.model->render(commandBuffer, pipeline->getLayout());
        }
    }

    void RenderSystem::setView(Camera::Type type) {
        auto& transform = camera->getComponent<Transform>();
        auto& cameraComponent = camera->getComponent<Camera>();
        switch (type) {
            case Camera::DIRECTION:
                cameraComponent.setViewDirection(transform.position, transform.eulerAngles);
                break;
            case Camera::LOOK_AT:
                // TODO: Implement look at Camera
                break;
        }
    }

    void RenderSystem::setProjection(float aspect) {
        auto& cameraComponent = camera->getComponent<Camera>();
        cameraComponent.setPerspectiveProjection(radians(50.f), aspect, 0.1f, 10.f);
    }

    std::shared_ptr<Entity> RenderSystem::getCamera() {
        return camera;
    }

} // namespace re