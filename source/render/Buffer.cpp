#include "Buffer.hpp"


namespace re {

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

        updateDescriptor();
    }

    Buffer::~Buffer() {
        vmaDestroyBuffer(allocator, buffer, allocation);
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

    void Buffer::updateDescriptor() {
        descriptor.offset = 0;
        descriptor.buffer = buffer;
        descriptor.range = size;
    }

} // namespace lv