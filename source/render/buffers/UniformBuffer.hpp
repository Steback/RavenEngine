#ifndef RAVENENGINE_UNIFORMBUFFER_HPP
#define RAVENENGINE_UNIFORMBUFFER_HPP


#include "Buffer.hpp"


namespace re {

    /**
     * @brief Uniform Buffer Object class wrappers
     */
    class UniformBuffer : public Buffer {
    public:
        /**
         * @brief Default constructor
         * @param allocator Vulkan Memory Allocator
         * @param size Buffer size
         * @param usageFlags Vulkan uboBuffer usage flags
         * @param memoryUsage Vulkan Memory Allocator memory usage
         */
        UniformBuffer(VmaAllocator allocator, VkDeviceSize size, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU);

        ~UniformBuffer();

        template<typename T>
        void update(T* data);
    };

    template<typename T>
    void UniformBuffer::update(T *data) {
        copyTo(data);
    }

} // namespace re


#endif //RAVENENGINE_UNIFORMBUFFER_HPP
