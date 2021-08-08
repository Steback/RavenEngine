#ifndef RAVENENGINE_DEVICE_HPP
#define RAVENENGINE_DEVICE_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"


namespace re {

    class Instance;

    /**
     * @brief Vulkan Physical Device and Device
     */
    class Device {
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
         */
        Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features);

        ~Device();

        /**
         * @brief Get the required Queue Family Index
         * @param queue The specific Queue Flag
         * @param surface Window surface for present support. By default is null
         * @return uint32_t value with the Queue Family Index
         */
        uint32_t getQueueFamilyIndex(VkQueueFlagBits queue, VkSurfaceKHR surface = VK_NULL_HANDLE);

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
         * @param queueFamilyIndex Specific Queue Family Index for the Command Pool
         * @param flags Create info flags. By default are VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT and VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
         */
        void createCommandPool(VkCommandPool& commandPool, uint32_t queueFamilyIndex,
                               uint32_t flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);

        /**
         * @brief Create a Command Buffer with single time record command
         * @param commandPool Command Pool for crate the Command Buffer. By default the Command Pool used are created with Transfer queue family index
         * @return The Command Buffer for record all the commands one time
         */
        VkCommandBuffer beginSingleTimeCommand(VkCommandPool commandPool = VK_NULL_HANDLE);

        /**
         * Submit commands to the queue and end command buffer
         * @param commandBuffer Command buffer with all recorded commands
         * @param queue Queue for submit the commands. By default is a Transfer queue.
         */
        void endSimpleTimeCommand(VkCommandBuffer commandBuffer, VkQueue queue = VK_NULL_HANDLE);

        /**
         *
         * @param queue Set the default Graphics queue to the referenced queue
         */
        void getGraphicsQueue(VkQueue& queue);

        /**
         *
         * @param queue Set the default Present queue if present index is different to graphics index, in that case the present queue will be the graphics queue
         */
        void getPresentQueue(VkQueue& queue);

        /**
         *
         * @param queue Set the default Compute queue to the referenced queue
         */
        void getComputeQueue(VkQueue& queue);

        /**
         *
         * @param queue Set the default Transfer queue to the referenced queue
         */
        void getTransferQueue(VkQueue& queue);

        /**
         *
         * @return Get the default vulkan device
         */
        [[nodiscard]] VkDevice getDevice() const;

        /**
         *
         * @return Get the default vulkan physical device
         */
        [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const;

        /**
         *
         * @return Get all the Queue family indices
         */
        [[nodiscard]] QueueFamilyIndices getQueueFamilyIndices() const;

    private:
        /**
         * Create the vulkan logical device
         * @param extensions Vector of C-Style string with all the extensions names required for the device
         * @param features Physical device features struct with the requires features
         * @param queueFlags The requires Queue flags, by default are Graphics, Compute and Transfer
         */
        void createLogicalDevice(const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
                                 VkQueueFlags queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);

    public:
        const float DEFAULT_QUEUE_PRIORITY = 1.0f;

    private:
        QueueFamilyIndices queueFamilyIndices{};
        VkDevice device{};
        VkPhysicalDevice physicalDevice{};
        VkQueue transferQueue{};
        VkCommandPool transferCmdPool{};
    };

} // namespace re


#endif //RAVENENGINE_DEVICE_HPP
