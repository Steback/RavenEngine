#ifndef RAVENENGINE_IMAGE_HPP
#define RAVENENGINE_IMAGE_HPP


#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#include "engine/core/NonCopyable.hpp"


namespace re {

    class SwapChain;

    class Image : NonCopyable {
        friend SwapChain;

    public:
        Image();

        Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage);

        Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage,
              VkImageAspectFlags aspectFlags);

        ~Image() override;

        void createView(VkImageAspectFlags aspectFlags, VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D, uint32_t layerCount = 1);

        void setLayout(
                VkCommandBuffer cmdbuffer,
                VkImageLayout oldImageLayout,
                VkImageLayout newImageLayout,
                VkImageSubresourceRange subresourceRange,
                VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        void setLayout(
                VkCommandBuffer cmdbuffer,
                VkImageAspectFlags aspectMask,
                VkImageLayout oldImageLayout,
                VkImageLayout newImageLayout,
                VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        [[nodiscard]] VkImage getImage() const;

        [[nodiscard]] VkImageView getView() const;

        [[nodiscard]] VkFormat getFormat() const;

        [[nodiscard]] VkExtent3D getExtent() const;

        [[nodiscard]] uint32_t getMipLevels() const;

    protected:
        void createImage(const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage);

    protected:
        VkDevice device{};
        VkImage image{};
        VkImageView view{};
        VkFormat format{};
        VkExtent3D extent{};
        uint32_t mipLevels{1};
        VmaAllocator allocator{};
        VmaAllocation allocation{};

    private:
        bool swapChainImages{false};
    };

} // namespace re


#endif //RAVENENGINE_IMAGE_HPP
