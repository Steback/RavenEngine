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
        unmap();
        vmaDestroyBuffer(allocator, buffer, allocation);
    }

    /**
     * @brief Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
     * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete buffer range.
     * @param offset (Optional) Byte offset from beginning
     * @return VkResult of the buffer mapping call
     */
    VkResult Buffer::map(VkDeviceSize size_, VkDeviceSize offset) {
        if (size_ == VK_WHOLE_SIZE)
            return  vmaMapMemory(allocator, allocation, &mapped);

        return vmaMapMemory(allocator, allocation, &mapped);
    }

    /**
     * @brief Copies the specified data to the mapped buffer. Default value writes whole buffer range
     * @param data Pointer to the data to copy
     * @param size [Optional] Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer range.
     * @param offset [Optional] Byte offset from beginning of mapped region
     */
    void Buffer::writeTo(void *data, VkDeviceSize size_, VkDeviceSize offset) {
        if (!mapped) return;

        if (size_ == VK_WHOLE_SIZE) {
            std::memcpy(mapped, data, size);
        } else {
            char *memOffset = (char *)mapped;
            memOffset += offset;
            std::memcpy(memOffset, data, size);
        }
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
     * @return Raw vulkan buffer
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
     * @brief Flush a memory range of the buffer to make it visible to the device
     * @note Only required for non-coherent memory
     * @param size [Optional] Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the complete buffer range.
     * @param offset [Optional] Byte offset from beginning
     */
    void Buffer::flush(VkDeviceSize size_, VkDeviceSize offset) {
        vmaFlushAllocation(allocator, allocation, offset, size_ == VK_WHOLE_SIZE ? size : size_);
    }

    /**
     * @brief Invalidate a memory range of the buffer to make it visible to the host
     * @note Only required for non-coherent memory
     * @param size [Optional] Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate the complete buffer range.
     * @param offset [Optional] Byte offset from beginning
     */
    void Buffer::invalidate(VkDeviceSize size_, VkDeviceSize offset) {
        vmaInvalidateAllocation(allocator, allocation, offset, size_ == VK_WHOLE_SIZE ? size : size_);
    }

    /**
     * @brief Create a buffer info descriptor
     * @param size [Optional] Size of the memory range of the descriptor
     * @param offset [Optional] Byte offset from beginning
     * @return VkDescriptorBufferInfo of specified offset and range
     */
    VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size_, VkDeviceSize offset) {
        return {buffer, offset, size_ == VK_WHOLE_SIZE ? size : size_};
    }

} // namespace lv