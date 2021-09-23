#ifndef RAVENENGINE_UNIFORMBUFFER_HPP
#define RAVENENGINE_UNIFORMBUFFER_HPP


#include "Buffer.hpp"


namespace re {

    // TODO: Add Doxygen comments
    class UniformBuffer : public Buffer {
    public:
        UniformBuffer(VmaAllocator allocator, VkDeviceSize size, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU);

        ~UniformBuffer();

        template<typename T>
        void update(T* data);
    };

    template<typename T>
    void UniformBuffer::update(T *data) {
        writeTo(data);
    }

} // namespace re


#endif //RAVENENGINE_UNIFORMBUFFER_HPP
