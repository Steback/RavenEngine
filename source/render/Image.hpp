#ifndef RAVENENGINE_IMAGE_HPP
#define RAVENENGINE_IMAGE_HPP


#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class SwapChain;

    /**
     * @brief Vulkan image wrapper
     */
    class Image : NonCopyable {
        friend SwapChain;

    public:
        /**
         * @brief Default constructor
         */
        Image();

        /**
         * @brief Explicit constructor, create Image, allocate its memory and create the image view
         * @param device Vulkan Device
         * @param allocator Vulkan Memory Allocator
         * @param imageInfo Vulkan Image create info
         * @param usage Vulkan Memory Allocator memory usage
         */
        Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage);

        /**
         * @brief Explicit constructor, create Image, allocate its memory and create the image view
         * @param device Vulkan Device
         * @param allocator Vulkan Memory Allocator
         * @param imageInfo Vulkan Image create info
         * @param usage Vulkan Memory Allocator memory usage
         * @param aspectFlags Image View aspect flags
         */
        Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage,
              VkImageAspectFlags aspectFlags);

        ~Image() override;

        /**
         * @brief Create Image View
         * @param aspectFlags Image View aspect flags
         */
        void createView(VkImageAspectFlags aspectFlags, VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D, uint32_t layerCount = 1);

        /**
         * @brief Put an image memory barrier for setting an image layout on the sub resource into the given command buffer
         */
        void setLayout(
                VkCommandBuffer cmdbuffer,
                VkImageLayout oldImageLayout,
                VkImageLayout newImageLayout,
                VkImageSubresourceRange subresourceRange,
                VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        /**
         * @brief Uses a fixed sub resource layout with first mip level and layer
         */
        void setLayout(
                VkCommandBuffer cmdbuffer,
                VkImageAspectFlags aspectMask,
                VkImageLayout oldImageLayout,
                VkImageLayout newImageLayout,
                VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        /**
         *
         * @return Vulkan Image
         */
        [[nodiscard]] VkImage getImage() const;

        /**
         *
         * @return Vulkan Image View
         */
        [[nodiscard]] VkImageView getView() const;

        /**
         *
         * @return Image Format
         */
        [[nodiscard]] VkFormat getFormat() const;

        /**
         *
         * @return Image 3D extent
         */
        [[nodiscard]] VkExtent3D getExtent() const;

        /**
         *
         * @return Current Image mip levels
         */
        [[nodiscard]] uint32_t getMipLevels() const;

    protected:
        /**
         * @brief Create Image and its Allocation
         * @param imageInfo Vulkan Image create info
         * @param usage Vulkan Memory Allocation memory usage
         */
        void createImage(const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage);

    protected:
        VkDevice device{};
        VkImage image{};
        VkImageView view{};
        VkFormat format{};
        VkExtent3D extent{};
        uint32_t mipLevels{1};

    private:
        VmaAllocator allocator{};
        VmaAllocation allocation{};
        bool swapChainImages{false};
    };

} // namespace re


#endif //RAVENENGINE_IMAGE_HPP
