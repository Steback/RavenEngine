#include "Image.hpp"

#include "engine/core/Utils.hpp"


namespace re {

    Image::Image() = default;

    /**
     * @brief Create Image and allocate its memory. Not create Image view.
     * @param device Raw Vulkan device
     * @param allocator Raw VMA allocator
     * @param imageInfo Vulkan Image create info
     * @param usage VMA memory usage
     */
    Image::Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo &imageInfo, VmaMemoryUsage usage)
            : device(device), allocator(allocator) {
        createImage(imageInfo, usage);
    }

    /**
     * @brief Create image, image view and allocate memory
     * @param device Raw Vulkan device
     * @param allocator Raw VMA allocator
     * @param imageInfo Vulkan Image create info
     * @param usage VMA memory usage
     * @param aspectFlags Image view aspect flags
     */
    Image::Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo &imageInfo, VmaMemoryUsage usage, VkImageAspectFlags aspectFlags)
            : device(device), allocator(allocator) {
        createImage(imageInfo, usage);
        createView(aspectFlags);
    }

    Image::~Image() {
        if (!swapChainImages) vkDestroyImageView(device, view, nullptr);
        if (!swapChainImages) vmaDestroyImage(allocator, image, allocation);
    }

    /**
     * @brief Create image view
     * @param aspectFlags Image view aspect flags
     * @param type [Optinal] Image view type. By default is 2D.
     * @param layerCount [Optional] Image layer count. By default is 1
     */
    void Image::createView(VkImageAspectFlags aspectFlags, VkImageViewType type, uint32_t layerCount) {
        VkImageViewCreateInfo viewInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        viewInfo.image = image;
        viewInfo.format = format;
        viewInfo.viewType = type;
        viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        viewInfo.subresourceRange = { aspectFlags, 0, mipLevels, 0, layerCount };

        checkResult(vkCreateImageView(device, &viewInfo, nullptr, &view),
                    "Failed to create Image View");
    }

    /**
     * @brief Set Image layout
     * @param cmdBuffer Valid command buffer in recording state
     * @param oldImageLayout Old layout
     * @param newImageLayout New layout
     * @param subresourceRange Sub resource range
     * @param srcStageMask [Optional] Pipeline source stage
     * @param dstStageMask [Optional] Pipeline destination stage
     */
    void Image::setLayout(VkCommandBuffer cmdBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange,
                          VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {
        // Create an image barrier object
        VkImageMemoryBarrier imageMemoryBarrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        imageMemoryBarrier.oldLayout = oldImageLayout;
        imageMemoryBarrier.newLayout = newImageLayout;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange = subresourceRange;

        // Source layouts (old)
        // Source access mask controls actions that have to be finished on the old layout
        // before it will be transitioned to the new layout
        switch (oldImageLayout) {
            case VK_IMAGE_LAYOUT_UNDEFINED:
                // Image layout is undefined (or does not matter)
                // Only valid as initial layout
                // No flags required, listed only for completeness
                imageMemoryBarrier.srcAccessMask = 0;
                break;

            case VK_IMAGE_LAYOUT_PREINITIALIZED:
                // Image is preinitialized
                // Only valid as initial layout for linear images, preserves memory contents
                // Make sure host writes have been finished
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                // Image is a color attachment
                // Make sure any writes to the color uboBuffer have been finished
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                // Image is a depth/stencil attachment
                // Make sure any writes to the depth/stencil uboBuffer have been finished
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                // Image is a transfer source
                // Make sure any reads from the image have been finished
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;

            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                // Image is a transfer destination
                // Make sure any writes to the image have been finished
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                // Image is read by a shader
                // Make sure any shader reads from the image have been finished
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            default:
                // Other source layouts aren't handled (yet)
                break;
        }

        // Target layouts (new)
        // Destination access mask controls the dependency for the new image layout
        switch (newImageLayout) {
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                // Image will be used as a transfer destination
                // Make sure any writes to the image have been finished
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                // Image will be used as a transfer source
                // Make sure any reads from the image have been finished
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;

            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                // Image will be used as a color attachment
                // Make sure any writes to the color uboBuffer have been finished
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                // Image layout will be used as a depth/stencil attachment
                // Make sure any writes to depth/stencil uboBuffer have been finished
                imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;

            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                // Image will be read in a shader (sampler, input attachment)
                // Make sure any writes to the image have been finished
                if (imageMemoryBarrier.srcAccessMask == 0)
                {
                    imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                }
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            default:
                // Other source layouts aren't handled (yet)
                break;
        }

        // Put barrier inside setup command uboBuffer
        vkCmdPipelineBarrier(
                cmdBuffer,
                srcStageMask,
                dstStageMask,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier);
    }

    /**
     * @brief Set Image layout with default sub resource range
     * @param cmdBuffer Valid command buffer in recording state
     * @param oldImageLayout Old layout
     * @param aspectMask Sub resource range aspect
     * @param newImageLayout New layout
     * @param subresourceRange Sub resource range
     * @param srcStageMask [Optional] Pipeline source stage
     * @param dstStageMask [Optional] Pipeline destination stage
     */
    void Image::setLayout(VkCommandBuffer cmdBuffer, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
                          VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {
        VkImageSubresourceRange subresourceRange{};
        subresourceRange.aspectMask = aspectMask;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.layerCount = 1;
        setLayout(cmdBuffer, oldImageLayout, newImageLayout, subresourceRange, srcStageMask, dstStageMask);
    }

    /**
     *
     * @return Raw Vulkan image
     */
    VkImage Image::getImage() const {
        return image;
    }

    /**
     *
     * @return Raw vulkan image view
     */
    VkImageView Image::getView() const {
        return view;
    }

    /**
     *
     * @return Image format
     */
    VkFormat Image::getFormat() const {
        return format;
    }

    /**
     *
     * @return Current image 3D extent
     */
    VkExtent3D Image::getExtent() const {
        return extent;
    }

    /**
     *
     * @return Image mipmap levels
     */
    uint32_t Image::getMipLevels() const {
        return mipLevels;
    }

    void Image::createImage(const VkImageCreateInfo &imageInfo, VmaMemoryUsage usage) {
        format = imageInfo.format;
        extent = imageInfo.extent;
        mipLevels = imageInfo.mipLevels;

        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = usage;

        checkResult(vmaCreateImage(allocator, &imageInfo, &allocationInfo, &image, &allocation, nullptr),
                    "Failed to create Image and memory Allocation");
    }

} // namespace re