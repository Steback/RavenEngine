#ifndef RAVENENGINE_UNIFORMBUFFER_HPP
#define RAVENENGINE_UNIFORMBUFFER_HPP


#include "Buffer.hpp"


namespace re {

    // TODO: Add Doxygen comments
    class UniformBuffer : public Buffer {
    public:
        UniformBuffer(VmaAllocator allocator, VkDeviceSize size, uint32_t instanceCount = 1, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU);

        ~UniformBuffer();

        void writeToIndex(void* data, int index);

        void flushIndex(int index);

        VkDescriptorBufferInfo descriptorInfoForIndex(int index);

        void invalidateIndex(int index);

    private:
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        uint32_t instanceCount;
        VkDeviceSize alignmentSize{};
    };

} // namespace re


#endif //RAVENENGINE_UNIFORMBUFFER_HPP
