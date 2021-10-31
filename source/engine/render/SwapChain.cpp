#include "SwapChain.hpp"

#include <limits>

#include "Device.hpp"
#include "Image.hpp"
#include "Instance.hpp"
#include "engine/core/Utils.hpp"


namespace re {

    /**
     *
     * @param device Pointer to Device
     * @param windowExtent Valid windows extent
     */
    SwapChain::SwapChain(const std::shared_ptr<Device>& device, VkExtent2D windowExtent)
            : device(device), windowExtent(windowExtent) {
        logicalDevice = device->getDevice();

        Device::QueueFamilyIndices indices = device->getQueueFamilyIndices();
        vkGetDeviceQueue(device->getDevice(), indices.graphics, 0, &graphicsQueue);

        if (indices.graphics != indices.present) {
            presentQueue = device->getQueue(static_cast<int32_t>(indices.present));
        } else {
            presentQueue = graphicsQueue;
        }

        init();
    }

    /**
     *
     * @param device Pointer to Device
     * @param windowExtent Valid windows extent
     * @param oldSwapChain Old SwapChain to use when SwapChain is recreated
     */
    SwapChain::SwapChain(const std::shared_ptr<Device>& device, VkExtent2D windowExtent, std::shared_ptr<SwapChain>  oldSwapChain)
            : device(device), windowExtent(windowExtent), oldSwapChain(std::move(oldSwapChain)) {
        logicalDevice = device->getDevice();
        graphicsQueue = this->oldSwapChain->graphicsQueue;
        presentQueue = this->oldSwapChain->presentQueue;

        init();

        this->oldSwapChain = nullptr;
    }

    SwapChain::~SwapChain() {
        for (auto& image : images)
            vkDestroyImageView(logicalDevice, image->view, nullptr);

        images.clear();

        if (swapChain) {
            vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
            swapChain = nullptr;
        }

        for (auto& image : depthImages)
            image = nullptr;

        for (auto& framebuffer : framebuffers)
            vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);

        vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
        }
    }

    /**
     *
     * @param index Current image index
     * @return Current framebuffer
     */
    VkFramebuffer SwapChain::getFrameBuffer(uint32_t index) const {
        return framebuffers[index];
    }

    /**
     *
     * @return Raw vulkan render pass
     */
    VkRenderPass SwapChain::getRenderPass() const {
        return renderPass;
    }

    /**
     *
     * @param index Current image index
     * @return Current image view
     */
    VkImageView SwapChain::getImageView(uint32_t index) const {
        return images[index]->view;
    }

    /**
     *
     * @return SwanChain images count
     */
    size_t SwapChain::getImageCount() const {
        return images.size();
    }

    /**
     *
     * @return SwapChain format
     */
    VkFormat SwapChain::getFormat() const {
        return format;
    }

    /**
     *
     * @return SwapChain extent
     */
    VkExtent2D SwapChain::getExtent() const {
        return extent;
    }

    /**
     *
     * @return Aspect ratio of current extent
     */
    float SwapChain::getExtentAspectRatio() const {
        return static_cast<float>(extent.width) / static_cast<float>(extent.height);
    }

    /**
     * @brief Acquire nex image to begin frame render
     * @param imageIndex Reference to image index to set the current index
     * @return Vulkan result with success value or log code if failed
     */
    VkResult SwapChain::acquireNextImage(uint32_t &imageIndex) {
        vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

        return vkAcquireNextImageKHR(
                logicalDevice,
                swapChain,
                std::numeric_limits<uint64_t>::max(),
                imageAvailableSemaphores[currentFrame],
                VK_NULL_HANDLE,
                &imageIndex
        );
    }

    /**
     * @brief Submit command buffers to draw frame to present queue
     * @param commandBuffer Valid command buffer
     * @param imageIndex Current image index
     * @return Vulkan result with success value or log code if failed
     */
    VkResult SwapChain::submitCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());

        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);
        checkResult(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]),
                    "Failed to submit draw command uboBuffer!");

        VkPresentInfoKHR presentInfo = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapChain;
        presentInfo.pImageIndices = &imageIndex;

        auto result = vkQueuePresentKHR(presentQueue, &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    /**
     * @brief Compare format of two SwapChain in case change format in recreating SwapChain
     * @param other SwapChain instance
     */
    bool SwapChain::compareFormats(const SwapChain &other) const {
        return other.depthFormat == depthFormat && other.format == format;
    }

    /**
     *
     * @param device Raw vulkan physical device
     * @param surface Valid SwapChain surface
     * @return SwapChain SupportDetails with all supported data
     */
    SwapChain::SupportDetails SwapChain::querySurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;

    }

    void SwapChain::init() {
        depthFormat =  device->findSupportFormat(
                {VK_FORMAT_D24_UNORM_S8_UINT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        createSwapChain();
        createImageViews();
        createRenderPass();
        createDepthResources();
        createFramebuffers();
        createSyncObjects();
    }

    void SwapChain::createSwapChain() {
        VkSurfaceKHR surface = device->getSurface();
        SupportDetails support = querySurfaceSupport(device->getPhysicalDevice(), surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(support.formats);
        VkPresentModeKHR presentMode = choosePresentMode(support.presentModes);
        VkExtent2D extent_ = chooseExtent(support.capabilities);

        uint32_t imageCount = support.capabilities.minImageCount + 1;
        if (support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount)
            imageCount = support.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent_;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        Device::QueueFamilyIndices indices = device->getQueueFamilyIndices();
        if (indices.graphics != indices.present) {
            uint32_t queueFamilyIndices[] = {indices.graphics, indices.present};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = support.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = ( !oldSwapChain ? VK_NULL_HANDLE : oldSwapChain->swapChain);

        checkResult(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain),
                    "Failed to create swap chain!");

        vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
        std::vector<VkImage> swapChainImages(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

        // TODO: Change creation of SwapChain Images
        images.resize(imageCount);
        for (size_t i = 0; i < imageCount; ++i) {
            images[i] = std::make_unique<Image>();
            images[i]->swapChainImages = true;
            images[i]->format = format;
            images[i]->image = swapChainImages[i];
        }

        format = surfaceFormat.format;
        extent = extent_;
    }

    void SwapChain::createImageViews() {
        VkImageViewCreateInfo viewInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        for (auto& image : images) {
            viewInfo.image = image->image;

            checkResult(vkCreateImageView(logicalDevice, &viewInfo, nullptr, &image->view),
                        "Failed to create texture image view!");
        }
    }

    void SwapChain::createDepthResources() {
        depthImages.resize(images.size());

        VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = extent.width;
        imageInfo.extent.height = extent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = depthFormat;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.flags = 0;

        for (auto& image : depthImages) {
            image = std::make_unique<Image>(
                    logicalDevice,
                    device->getAllocator(),
                    imageInfo,
                    VMA_MEMORY_USAGE_GPU_ONLY,
                    VK_IMAGE_ASPECT_DEPTH_BIT
            );
        }
    }

    void SwapChain::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = depthFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.dstSubpass = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

        std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
        VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
        renderPassInfo.attachmentCount = attachments.size();
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        checkResult(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass),
                    "Failed to create render pass!");
    }

    void SwapChain::createFramebuffers() {
        VkFramebufferCreateInfo framebufferInfo = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        framebuffers.resize(getImageCount());
        for (size_t i = 0; i < getImageCount(); i++) {
            std::array<VkImageView, 2> attachments = {images[i]->view, depthImages[i]->view};
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments = attachments.data();

            checkResult(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &framebuffers[i]),
                        "failed to create framebuffer!");
        }
    }

    void SwapChain::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(getImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

        VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            checkResult(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]),
                        "Failed to create synchronization objects for a frame!");

            checkResult(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]),
                        "Failed to create synchronization objects for a frame!");

            checkResult(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]),
                        "Failed to create synchronization objects for a frame!");
        }
    }

    VkSurfaceFormatKHR SwapChain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto &availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        }

        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::choosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto &availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::chooseExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = windowExtent;
            actualExtent.width = std::max(
                    capabilities.minImageExtent.width,
                    std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                    capabilities.minImageExtent.height,
                    std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }

    }

} // namespace re