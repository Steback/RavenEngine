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

        cameraEntity = scene.addEntity("Camera");
        cameraEntity->addComponent<Transform>(vec3{}, vec3{}, vec3{});

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
        pipeline->bind(commandBuffer);

        mat4 viewProj = camera.getProjection() * camera.getView();

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
        auto& transform = cameraEntity->getComponent<Transform>();
        switch (type) {
            case Camera::DIRECTION:
                camera.setViewDirection(transform.position, transform.rotation);
                break;
            case Camera::LOOK_AT:
                // TODO: Implement look at Camera
                break;
        }
    }

    void RenderSystem::setProjection(float aspect) {
        camera.setPerspectiveProjection(radians(50.f), aspect, 0.1f, 10.f);
    }

} // namespace re