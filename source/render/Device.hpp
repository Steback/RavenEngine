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

    class Device : NonCopyable {
    public:
        struct QueueFamilyIndices {
            uint32_t graphics;
            uint32_t present;
            uint32_t compute;
            uint32_t transfer;
        };

        Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
               VkSurfaceKHR surface = VK_NULL_HANDLE);

        ~Device() override;

        uint32_t getQueueFamilyIndex(VkQueueFlagBits queue, VkSurfaceKHR surface_ = VK_NULL_HANDLE);

        VkFormat findSupportFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        void createCommandPool(VkCommandPool& commandPool, uint32_t index, uint32_t flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);

        VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool = VK_NULL_HANDLE);

        void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue = VK_NULL_HANDLE, VkCommandPool commandPool = VK_NULL_HANDLE);

        void copyBuffer(Buffer& src, Buffer& dst, VkDeviceSize size);

        void copyBufferToImage(Buffer& src, Image& dst);

        void transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1);

        [[nodiscard]] VkDevice getDevice() const;

        [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const;

        [[nodiscard]] QueueFamilyIndices getQueueFamilyIndices() const;

        [[nodiscard]] VmaAllocator getAllocator() const;

        [[nodiscard]] std::shared_ptr<Instance> getInstance() const;

        [[nodiscard]] VkSurfaceKHR getSurface() const;

        VkQueue getQueue(int32_t index = -1);

        VkCommandPool getCommandPool(int32_t index = -1);

    private:
        void createLogicalDevice(const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features);

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
