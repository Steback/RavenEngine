#ifndef RAVENENGINE_RENDERSYSTEM_HPP
#define RAVENENGINE_RENDERSYSTEM_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class Device;
    class GraphicsPipeline;
    class Scene;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName,
                     const std::vector<VkPushConstantRange>& pushConstantsRanges);

        ~RenderSystem() override;

        void renderScene(VkCommandBuffer commandBuffer, Scene& scene);

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
