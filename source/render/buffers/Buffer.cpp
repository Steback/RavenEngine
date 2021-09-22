#include "Buffer.hpp"


namespace re {

    /**
     * @brief Create buffer and allocate its memory
     * @param allocator VMA Allocator
     * @param size Buffer size
     * @param usageFlags Buffer usage flags
     * @param memoryUsage VMA buffer memory usage
     */
    Buffer::Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage)
            : allocator(allocator), size(size) {
        VkBufferCreateInfo bufferCreateInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferCreateInfo.usage = usageFlags;
        bufferCreateInfo.size = size;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferCreateInfo.flags = 0;

        VmaAllocationCreateInfo allocationCreateInfo{};
        allocationCreateInfo.usage = memoryUsage;

        vmaCreateBuffer(allocator, &bufferCreateInfo, &allocationCreateInfo, &buffer, &allocation, nullptr);
    }

    Buffer::~Buffer() {
        vmaDestroyBuffer(allocator, buffer, allocation);
    }

    /**
     * @brief Map a memory range
     */
    void Buffer::map() {
        vmaMapMemory(allocator, allocation, &mapped);
    }

    /**
     * @brief Unmap a mapped memory range
     */
    void Buffer::unmap() {
        if (mapped) {
            vmaUnmapMemory(allocator, allocation);
            mapped = nullptr;
        }
    }

    /**
     *
     * @return Vulkan buffer
     */
    const VkBuffer &Buffer::getBuffer() const {
        return buffer;
    }

    /**
     *
     * @return Buffer size
     */
    VkDeviceSize Buffer::getSize() const {
        return size;
    }

    /**
     *
     * @return Vulkan descriptor buffer info
     */
    VkDescriptorBufferInfo Buffer::getDescriptor() {
        return {buffer, 0, size};
    }

} // namespace lv