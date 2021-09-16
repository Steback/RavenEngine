#include "Device.hpp"

#include "Instance.hpp"
#include "Buffer.hpp"
#include "Image.hpp"
#include "utils/Utils.hpp"


namespace re {

    Device::Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
                   VkSurfaceKHR surface) : instance(instance), surface(surface) {
        physicalDevice = instance->pickPhysicalDevice(extensions);

        createLogicalDevice(extensions, features);

        std::vector<uint32_t> indices = {
                queueFamilyIndices.graphics,
                queueFamilyIndices.present,
                queueFamilyIndices.transfer,
                queueFamilyIndices.compute
        };

        for (auto& index : indices) {
            if (queues.find(index) == queues.end()) {
                if (index == queueFamilyIndices.graphics) {
                    vkGetDeviceQueue(device, index, 1, &queues[index]);
                } else {
                    vkGetDeviceQueue(device, index, 0, &queues[index]);
                }
            }

            if (commandPools.find(index) == commandPools.end())
                createCommandPool(commandPools[index], index);
        }

        createAllocator(instance);
    }

    Device::~Device() {
        vkDestroySurfaceKHR(instance->getInstance(), surface, nullptr);
        vmaDestroyAllocator(allocator);

        for (auto& [index, commandPool] : commandPools) {
            vkDestroyCommandPool(device, commandPool, nullptr);
        }

        vkDestroyDevice(device, nullptr);
    }

    uint32_t Device::getQueueFamilyIndex(VkQueueFlagBits queue, VkSurfaceKHR surface_) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> properties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, properties.data());

        if (queue & VK_QUEUE_COMPUTE_BIT) {
            for (uint32_t i = 0; i < properties.size(); ++i) {
                if ((properties[i].queueFlags & queue) && (!(properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)))
                    return i;
            }
        }

        if (queue & VK_QUEUE_TRANSFER_BIT) {
            for (uint32_t i = 0; i < properties.size(); ++i) {
                if ((properties[i].queueFlags & queue) && (!(properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)))
                    return i;
            }
        }

        for (uint32_t i = 0; i < properties.size(); ++i) {
            if (properties[i].queueFlags & queue) {
                if (surface_) {
                    VkBool32 supported = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface_, &supported);

                    if (supported) return i;
                }

                return i;
            }
        }

        throwEx("Could not find a matching queue family index");
        return 0;
    }

    VkFormat Device::findSupportFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                       VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throwEx("Failed to find supported format!");
        return VK_FORMAT_R8G8B8A8_SNORM;
    }

    void Device::createCommandPool(VkCommandPool &commandPool, uint32_t index, uint32_t flags) {
        VkCommandPoolCreateInfo createInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        createInfo.flags = flags;

        if (index == queueFamilyIndices.compute) {
            createInfo.queueFamilyIndex = queueFamilyIndices.compute;
            vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
        } else if (index == queueFamilyIndices.transfer) {
            createInfo.queueFamilyIndex = queueFamilyIndices.transfer;
            vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
        } else {
            createInfo.queueFamilyIndex = queueFamilyIndices.graphics;
            vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
        }
    }

    VkCommandBuffer Device::beginSingleTimeCommands(VkCommandPool commandPool) {
        VkCommandBufferAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool = (commandPool ? commandPool : commandPools[queueFamilyIndices.graphics]);
        allocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        return commandBuffer;
    }

    void Device::endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue_, VkCommandPool commandPool) {
        VkQueue queue = queue_ ? queue_ : queues[queueFamilyIndices.graphics];

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(device, (commandPool ? commandPool : commandPools[queueFamilyIndices.graphics]), 1, &commandBuffer);
    }

    void Device::copyBuffer(Buffer &src, Buffer &dst, VkDeviceSize size) {
        VkQueue queue = getQueue(static_cast<int32_t>(queueFamilyIndices.transfer));
        VkCommandPool commandPool = getCommandPool(static_cast<int32_t>(queueFamilyIndices.transfer));
        VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, src.getBuffer(), dst.getBuffer(), 1, &copyRegion);

        endSingleTimeCommands(commandBuffer, queue, commandPool);
    }

    void Device::copyBufferToImage(Buffer& src, Image& dst) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {dst.getExtent().width, dst.getExtent().height, 1};
        vkCmdCopyBufferToImage(commandBuffer, src.getBuffer(), dst.getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        endSingleTimeCommands(commandBuffer);
    }

    void Device::transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throwEx("Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage, destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
        );

        endSingleTimeCommands(commandBuffer);
    }

    VkDevice Device::getDevice() const {
        return device;
    }

    VkPhysicalDevice Device::getPhysicalDevice() const {
        return physicalDevice;
    }

    Device::QueueFamilyIndices Device::getQueueFamilyIndices() const {
        return queueFamilyIndices;
    }

    VmaAllocator Device::getAllocator() const {
        return allocator;
    }

    std::shared_ptr<Instance> Device::getInstance() const {
        return instance;
    }

    VkSurfaceKHR Device::getSurface() const {
        return surface;
    }

    VkQueue Device::getQueue(int32_t index) {
        return queues[index > -1 ? index : queueFamilyIndices.graphics];
    }

    VkCommandPool Device::getCommandPool(int32_t index) {
        return commandPools[index > -1 ? index : queueFamilyIndices.graphics];
    }

    void Device::createLogicalDevice(const std::vector<const char *>& extensions, VkPhysicalDeviceFeatures features) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

        queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        queueFamilyIndices.present = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, surface);

        {
            if (queueFamilyIndices.graphics == queueFamilyIndices.present) {
                // The first queue will use only by the SwapChain. the second will be used for other thing such generated
                // texture mipmaps
                const float priorityQueues[] = {DEFAULT_QUEUE_PRIORITY, DEFAULT_QUEUE_PRIORITY};
                VkDeviceQueueCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                createInfo.queueFamilyIndex = queueFamilyIndices.graphics;
                createInfo.queueCount = 2;
                createInfo.pQueuePriorities = priorityQueues;
                queueCreateInfos.push_back(createInfo);
            } else {
                VkDeviceQueueCreateInfo createInfoGraphics{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                createInfoGraphics.queueFamilyIndex = queueFamilyIndices.graphics;
                createInfoGraphics.queueCount = 1;
                createInfoGraphics.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
                queueCreateInfos.push_back(createInfoGraphics);

                VkDeviceQueueCreateInfo createInfoPresent{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                createInfoPresent.queueFamilyIndex = queueFamilyIndices.present;
                createInfoPresent.queueCount = 1;
                createInfoPresent.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
                queueCreateInfos.push_back(createInfoPresent);
            }
        }

        {
            queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
            VkDeviceQueueCreateInfo computeCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
            computeCreateInfo.queueFamilyIndex = queueFamilyIndices.compute;
            computeCreateInfo.queueCount = 1;
            computeCreateInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
            queueCreateInfos.push_back(computeCreateInfo);
        }

        {
            queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
            VkDeviceQueueCreateInfo transferCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
            transferCreateInfo.queueFamilyIndex = queueFamilyIndices.transfer;
            transferCreateInfo.queueCount = 1;
            transferCreateInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
            queueCreateInfos.push_back(transferCreateInfo);
        }

        VkDeviceCreateInfo deviceInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        deviceInfo.enabledExtensionCount = extensions.size();
        deviceInfo.ppEnabledExtensionNames = extensions.data();
        deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceInfo.pEnabledFeatures = &features;

        checkResult(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device),
                    "Failed to create logical device!");
    }

    void Device::createAllocator(const std::shared_ptr<Instance> &instance_) {
        VmaAllocatorCreateInfo allocatorInfo{};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        allocatorInfo.device = device;
        allocatorInfo.physicalDevice = physicalDevice;
        allocatorInfo.instance = instance_->getInstance();

        checkResult(vmaCreateAllocator(&allocatorInfo, &allocator),
                    "Failed to create Vulkan Memory Allocator");
    }

} // namespace re