#ifndef LITTLEVULKANENGINE_BUFFER_HPP
#define LITTLEVULKANENGINE_BUFFER_HPP


#include <cstring>

#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class Buffer {
    public:
        Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage);

        ~Buffer();

        void map();

        template<typename T>
        void copyTo(T* data) const;

        void unmap();

        [[nodiscard]] const VkBuffer& getBuffer() const;

        [[nodiscard]] VkDeviceSize getSize() const;

        VkDescriptorBufferInfo getDescriptor();

    private:
        VmaAllocator allocator{};
        VkBuffer buffer{};
        VmaAllocation allocation{};
        VkDeviceSize size{};
        void* mapped{nullptr};
    };

    template<typename T>
    void Buffer::copyTo(T *data) const {
        std::memcpy(mapped, data, size);
    }

} // namespace re


#endif //LITTLEVULKANENGINE_BUFFER_HPP
