#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"


namespace re {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName,
                               const std::vector<VkPushConstantRange>& pushConstantsRanges)
            : device(std::move(device)) {

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert.spv", shadersName + ".frag.spv",
                configInfo,
                pushConstantsRanges
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::renderEntities(VkCommandBuffer commandBuffer) {
        pipeline->bind(commandBuffer);
    }

} // namespace re