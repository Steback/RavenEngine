#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


namespace re {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName, Scene& scene)
            : device(std::move(device)), scene(scene) {

        auto cameraEntity = scene.addEntity("Camera");
        cameraEntity->addComponent<Transform>(vec3{}, vec3{}, vec3{});

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert.spv", shadersName + ".frag.spv",
                configInfo,
                std::vector<VkPushConstantRange>{}
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer) {
        pipeline->bind(commandBuffer);

        for (auto& id : scene.getRegistry().view<Transform, MeshRender>()) {
            auto entity = scene.getEntity(id);
            auto& transform = entity->getComponent<Transform>();
            auto& meshRender = entity->getComponent<MeshRender>();
            meshRender.model->render(commandBuffer, pipeline->getLayout());
        }
    }

} // namespace re