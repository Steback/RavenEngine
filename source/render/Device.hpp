#ifndef RAVENENGINE_DEVICE_HPP
#define RAVENENGINE_DEVICE_HPP


#include <memory>
#include <vector>
#include <unordered_map>

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class Instance;
    class Buffer;
    class Image;

    /**
     * @brief Vulkan Physical Device and Device
     */
    class Device : NonCopyable {
    public:
        struct QueueFamilyIndices {
            uint32_t graphics;
            uint32_t present;
            uint32_t compute;
            uint32_t transfer;
        };

    public:
        /**
         *
         * @param instance shared_ptr of Instance class
         * @param extensions vector cf C-Style strings with the extensions names
         * @param features Physical Device features
         * @param surface Window surface for query present support by default is null
         */
        Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
               VkSurfaceKHR surface = VK_NULL_HANDLE);

        ~Device() override;

        /**
         * @brief Get the required Queue Family Index
         * @param queue The specific Queue Flag
         * @param surface_ Window surface for present support. By default is null
         * @return uint32_t value with the Queue Family Index
         */
        uint32_t getQueueFamilyIndex(VkQueueFlagBits queue, VkSurfaceKHR surface_ = VK_NULL_HANDLE);

        /**
         * @brief Find a format that support the Image tiling and the Features.
         * @param candidates Vector with all the available formats
         * @param tiling The specific image tilings
         * @param features The specific format features
         * @return The required format if exists
         */
        VkFormat findSupportFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        /**
         *
         * @param commandPool Reference of the commandPool to create
         * @param index Specific Queue Family Index for the Command Pool
         * @param flags Create info flags. By default are VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT and VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
         */
        void createCommandPool(VkCommandPool& commandPool, uint32_t index, uint32_t flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);

        /**
         * @brief Create a Command Buffer with single time record command
         * @param commandPool Command Pool for crate the Command Buffer. By default the Command Pool used are created with Graphics queue family index.
         * @return The Command Buffer for record all the commands one time
         */
        VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool = VK_NULL_HANDLE);

        /**
         * Submit commands to the queue and end command buffer
         * @param commandBuffer Command buffer with all recorded commands
         * @param queue Queue for submit the commands. By default is a Graphics queue.
         * @param commandPool Command Pool for destroy Command Buffer. By default the Command Pool used are created with Graphics queue family index.
         */
        void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue = VK_NULL_HANDLE, VkCommandPool commandPool = VK_NULL_HANDLE);

        /**
         * @brief Copy Buffer from other Buffer\n
         * @param src Source buffer
         * @param dst Destination buffer
         * @param size Buffer size. Must be equal to both buffers
         */
        void copyBuffer(Buffer& src, Buffer& dst, VkDeviceSize size);

        /**
         * @brief Copy buffer to Image
         * @param src Source Buffer
         * @param dst Destination Image
         */
        void copyBufferToImage(Buffer& src, Image& dst);

        void transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1);

        [[nodiscard]] VkDevice getDevice() const;

        [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const;

        [[nodiscard]] QueueFamilyIndices getQueueFamilyIndices() const;

        [[nodiscard]] VmaAllocator getAllocator() const;

        [[nodiscard]] std::shared_ptr<Instance> getInstance() const;

        [[nodiscard]] VkSurfaceKHR getSurface() const;

        /**
         *
         * @param index Queue Family Index. By default is Graphics.
         */
        VkQueue getQueue(int32_t index = -1);

        /**
         *
         * @param index Queue Family Index. By default is Graphics.
         */
        VkCommandPool getCommandPool(int32_t index = -1);

    private:
        void createLogicalDevice(const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
                                 VkQueueFlags queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);

        void createAllocator(const std::shared_ptr<Instance>& instance_);

    public:
        const float DEFAULT_QUEUE_PRIORITY = 1.0f;

    private:
        std::shared_ptr<Instance> instance;
        VkSurfaceKHR surface{};
        QueueFamilyIndices queueFamilyIndices{};
        VkDevice device{};
        VkPhysicalDevice physicalDevice{};
        std::unordered_map<uint32_t, VkQueue> queues;
        std::unordered_map<uint32_t, VkCommandPool> commandPools;
        VmaAllocator allocator{};
    };

} // namespace re


#endif //RAVENENGINE_DEVICE_HPP
