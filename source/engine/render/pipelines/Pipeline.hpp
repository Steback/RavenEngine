#ifndef LITTLEVULKANENGINE_PIPELINE_HPP
#define LITTLEVULKANENGINE_PIPELINE_HPP


#include <vector>

#include "vulkan/vulkan.h"

#include "engine/core/NonCopyable.hpp"


namespace re {

    /**
     * @brief Pure virtual Pipeline bae class
     */
    class Pipeline {
    public:
        /**
         * @brief Vulkan Pipeline creating config info\n
         * Contain all the necessary structs, Render pass and subpass for setup a Vulkan Pipeline
         */
        struct ConfigInfo : NonCopyable {
            VkPipelineViewportStateCreateInfo viewportInfo{};
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
            VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
            VkPipelineMultisampleStateCreateInfo multisampleInfo{};
            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
            VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
            std::vector<VkDynamicState> dynamicStateEnables;
            VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
            VkRenderPass renderPass{nullptr};
            uint32_t subpass{0};
        };

    protected:
        Pipeline() = default;

        ~Pipeline() = default;

        virtual void bind(const VkCommandBuffer& commandBuffer) const = 0;

        [[nodiscard]] virtual const VkPipeline& getPipeline() const = 0;

        [[nodiscard]] virtual const VkPipelineLayout& getLayout() const = 0;
    };

} // namespace re


#endif //LITTLEVULKANENGINE_PIPELINE_HPP
