#include "Buffer.hpp"


namespace re {

    Buffer::Buffer() = default;

    Buffer::Buffer(VmaAllocator allocator) : allocator(allocator) {

    }

    Buffer::~Buffer() = default;

    void Buffer::destroy() {
        vmaDestroyBuffer(allocator, buffer, allocation);
    }

    void Buffer::allocateMemory(VkDeviceSize size_, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage) {
        size = size_;
        VkBufferCreateInfo bufferCreateInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferCreateInfo.usage = usageFlags;
        bufferCreateInfo.size = size;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferCreateInfo.flags = 0;

        VmaAllocationCreateInfo allocationCreateInfo{};
        allocationCreateInfo.usage = memoryUsage;

        vmaCreateBuffer(allocator, &bufferCreateInfo, &allocationCreateInfo, &buffer, &allocation, nullptr);
    }

    void Buffer::map() {
        vmaMapMemory(allocator, allocation, &mapped);
    }

    void Buffer::unmap() {
        if (mapped) {
            vmaUnmapMemory(allocator, allocation);
            mapped = nullptr;
        }
    }

    const VkBuffer &Buffer::getBuffer() const {
        return buffer;
    }

    VkDeviceSize Buffer::getSize() const {
        return size;
    }

} // namespace lv