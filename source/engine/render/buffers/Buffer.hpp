#ifndef LITTLEVULKANENGINE_BUFFER_HPP
#define LITTLEVULKANENGINE_BUFFER_HPP


#include <cstring>

#include "vk_mem_alloc.h"

#include "engine/utils/NonCopyable.hpp"


namespace re {

    class Buffer {
    public:
        Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage);

        ~Buffer();

        VkResult map(VkDeviceSize size_ = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void writeTo(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void unmap();

        [[nodiscard]] const VkBuffer& getBuffer() const;

        [[nodiscard]] VkDeviceSize getSize() const;

        void flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    protected:
        VmaAllocator allocator{};
        VkBuffer buffer{};
        VmaAllocation allocation{};
        VkDeviceSize size{};
        void* mapped{nullptr};
    };

} // namespace re


#endif //LITTLEVULKANENGINE_BUFFER_HPP
