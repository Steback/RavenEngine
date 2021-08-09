#ifndef RAVENENGINE_IMAGE_HPP
#define RAVENENGINE_IMAGE_HPP


#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace re {

    /**
     * @brief Vulkan image wrapper
     */
    class Image : NonCopyable {
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
         * @param aspectFlags Image View aspect flags
         */
        Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage,
              VkImageAspectFlags aspectFlags);

        ~Image() override;

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

    private:
        /**
         * @brief Create Image and its Allocation
         * @param imageInfo Vulkan Image create info
         * @param usage Vulkan Memory Allocation memory usage
         */
        void createImage(const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage);

        /**
         * @brief Create Image View
         * @param aspectFlags Image View aspect flags
         */
        void createView(VkImageAspectFlags aspectFlags);

    private:
        VkDevice device{};
        VmaAllocator allocator{};
        VkImage image{};
        VkImageView view{};
        VmaAllocation allocation{};
        VkFormat format{};
        VkExtent3D extent{};
        uint32_t mipLevels{1};
    };

} // namespace re


#endif //RAVENENGINE_IMAGE_HPP
