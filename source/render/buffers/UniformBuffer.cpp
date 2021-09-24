#include "UniformBuffer.hpp"


namespace re {

    UniformBuffer::UniformBuffer(VmaAllocator allocator, VkDeviceSize size, uint32_t instanceCount, VmaMemoryUsage memoryUsage)
            : Buffer(allocator, size * instanceCount, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, memoryUsage), instanceCount(instanceCount) {
        map();
    }

    UniformBuffer::~UniformBuffer()= default;

    /**
     * @brief Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
     * @param data Pointer to the data to copy
     * @param index Used in offset calculation
     */
    void UniformBuffer::writeToIndex(void *data, int index) {
        writeTo(data, size, index * alignmentSize);
    }

    /**
     * @brief Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
     * @param index Used in offset calculation
     */
    void UniformBuffer::flushIndex(int index) {
        flush(alignmentSize, index * alignmentSize);
    }

    /**
     * @brief Create a buffer info descriptor
     * @param index Specifies the region given by index * alignmentSize
     * @return VkDescriptorBufferInfo for instance at index
     */
    VkDescriptorBufferInfo UniformBuffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(alignmentSize, index * alignmentSize);
    }

    /**
     * @brief Invalidate a memory range of the buffer to make it visible to the host
     * @note Only required for non-coherent memory
     * @param index Specifies the region to invalidate: index * alignmentSize
     */
    void UniformBuffer::invalidateIndex(int index) {
        invalidate(alignmentSize, index * alignmentSize);
    }

    VkDeviceSize UniformBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
        return 0;
    }

} // namespace re
