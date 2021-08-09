#include "Image.hpp"

#include "utils/Macros.hpp"


namespace re {

    Image::Image() = default;

    Image::Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo &imageInfo, VmaMemoryUsage usage, VkImageAspectFlags aspectFlags)
            : device(device), allocator(allocator) {
        createImage(imageInfo);
        createView(aspectFlags);
    }

    Image::~Image() {
        vkDestroyImageView(device, view, nullptr);
        vmaDestroyImage(allocator, image, allocation);
    }

    VkImage Image::getImage() const {
        return image;
    }

    VkImageView Image::getView() const {
        return view;
    }

    VkFormat Image::getFormat() const {
        return format;
    }

    VkExtent3D Image::getExtent() const {
        return extent;
    }

    uint32_t Image::getMipLevels() const {
        return mipLevels;
    }

    void Image::createImage(const VkImageCreateInfo &imageInfo, VmaMemoryUsage usage) {
        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = usage;

        RE_VK_CHECK_RESULT(vmaCreateImage(allocator, &imageInfo, &allocationInfo, &image, &allocation, nullptr),
                           "Failed to create Image and memory Allocation");
    }

    void Image::createView(VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        viewInfo.image = image;
        viewInfo.format = format;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.layerCount = 1;

        RE_VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &view),
                           "Failed to create Image View");
    }

} // namespace re