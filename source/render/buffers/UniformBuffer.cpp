#include "UniformBuffer.hpp"


namespace re {

    UniformBuffer::UniformBuffer(VmaAllocator allocator, VkDeviceSize size, VmaMemoryUsage memoryUsage)
            : Buffer(allocator, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, memoryUsage) {
        map();
    }

    UniformBuffer::~UniformBuffer(){
        unmap();
    }

} // namespace re
