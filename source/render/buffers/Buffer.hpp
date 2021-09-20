#ifndef LITTLEVULKANENGINE_BUFFER_HPP
#define LITTLEVULKANENGINE_BUFFER_HPP


#include <cstring>

#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace re {

    /**
     * @brief Buffer class
     */
    class Buffer {
    public:
        /**
         * @brief Default constructor
         * @param allocator Vulkan Memory Allocator
         * @param size Buffer size
         * @param usageFlags Vulkan uboBuffer usage flags
         * @param memoryUsage Vulkan Memory Allocator memory usage
         */
        Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage);

        ~Buffer();

        /**
         * @brief Map uboBuffer using Vulkan Memory Allocator
         */
        void map();

        /**
         * @brief Copy data to mapped pointer
         * @tparam T Data type
         * @param data Raw pointer of data
         */
        template<typename T>
        void copyTo(T* data) const;

        /**
         * @brief Unmap uboBuffer
         */
        void unmap();

        /**
         *
         * @return Raw vulkan uboBuffer
         */
        [[nodiscard]] const VkBuffer& getBuffer() const;

        /**
         *
         * @return Vulkan uboBuffer size
         */
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
