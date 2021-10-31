#include "Renderer.hpp"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "ui/ImGuiRender.hpp"
#include "engine/math/Vector2.hpp"
#include "engine/core/Utils.hpp"
#include "engine/config/Config.hpp"


namespace re {

    /**
     * @brief Create and setup all necessary render stuff
     * @param appName Application name
     * @param config Application config
     */
    Renderer::Renderer(const std::string& appName, const Config& config) {
        window = std::make_shared<Window>(appName, config.getWidth(), config.getHeight());

        std::vector<const char *> validationLayers;

#ifdef RE_DEBUG
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(appName.c_str(), validationLayers);

        VkSurfaceKHR surface;
        window->createSurface(instance->getInstance(), &surface);

        VkPhysicalDeviceFeatures features{};
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        device = std::make_shared<Device>(instance, extensions, features, surface);
        logicalDevice = device->getDevice();

        device->createCommandPool(commandPool, device->getQueueFamilyIndices().graphics);

        swapChain = std::make_unique<SwapChain>(device, window->getExtent());
        createCommandBuffers();

        imgui = std::make_unique<ui::ImGuiRender>(device, *swapChain, window);
    }

    Renderer::~Renderer() {
        freeCommandBuffers();
        vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
    }

    /**
     * @brief Check if window is open
     */
    bool Renderer::isWindowOpen() const {
        return window->isOpen();
    }

    /**
     *
     * @return Raw vulkan SwapChain render pass
     */
    VkRenderPass Renderer::getRenderPass() const {
        return swapChain->getRenderPass();
    }

    /**
     * @brief Check if current frame is in progress
     */
    bool Renderer::isFrameInProgress() const {
        return isFrameStarted;
    }

    /**
     *
     * @return Current command buffer used to record frame data
     */
    VkCommandBuffer Renderer::getCommandBuffer() const {
        return commandBuffers[currentFrameIndex];
    }

    /**
     *
     * @return Current frame index
     */
    int Renderer::getFrameIndex() const {
        return currentFrameIndex;
    }

    /**
     * @brief Acquire image and begin frame
     * @return Current command buffer used to record frame data
     */
    VkCommandBuffer Renderer::beginFrame() {
        auto result = swapChain->acquireNextImage(imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throwEx("Failed to acquire swap chain image!");

        isFrameStarted = true;

        auto commandBuffer = getCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        checkResult(vkBeginCommandBuffer(commandBuffer, &beginInfo),
                    "Failed to begin recording command uboBuffer!");

        return commandBuffer;
    }

    /**
     * @brief End frame and submit command buffer
     */
    void Renderer::endFrame() {
        auto commandBuffer = getCommandBuffer();

        checkResult(vkEndCommandBuffer(commandBuffer), "Failed to record command uboBuffer!");

        auto result = swapChain->submitCommandBuffers(commandBuffer, imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->wasWindowResized()) {
            window->resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throwEx("Failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    /**
     * @brief Begin render pass, set viewport and clear values
     * @param commandBuffer Current command buffer used to record frame data
     */
    void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        VkRenderPassBeginInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->getExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getExtent().width);
        viewport.height = static_cast<float>(swapChain->getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, swapChain->getExtent()};

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    /**
     * @brief End render pass
     * @param commandBuffer Current command buffer used to record frame data
     */
    void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) const {
        vkCmdEndRenderPass(commandBuffer);
    }

    /**
     * @brief Wait until all commands will be submitted
     */
    void Renderer::waitDeviceIde() {
        vkDeviceWaitIdle(logicalDevice);
    }

    /**
     *
     * @return Pointer to Device
     */
    std::shared_ptr<Device> Renderer::getDevice() const {
        return device;
    }

    /**
     *
     * @return Current windows aspect ratio
     */
    float Renderer::getAspectRatio() const {
        return swapChain->getExtentAspectRatio();
    }

    /**
     * @brief Begin ImGui frame
     */
    void Renderer::newImGuiFrame() {
        imgui->newFrame();
    }

    /**
     *
     * @param commandBuffer Current command buffer used to record frame data
     */
    void Renderer::renderImGui(VkCommandBuffer commandBuffer) {
        imgui->render(commandBuffer);
    }

    /**
     *
     * @return Get SwapChain images count
     */
    uint32_t Renderer::getImageCount() const {
        return swapChain->getImageCount();
    }

    Vector2 Renderer::getWindowSize() const {
        return window->getSize();
    }

    void Renderer::createCommandBuffers() {
        commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = commandBuffers.size();

        checkResult(vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()),
                    "Failed to allocate command buffers!");
    }

    void Renderer::freeCommandBuffers() {
        vkFreeCommandBuffers(logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
        commandBuffers.clear();
    }

    void Renderer::recreateSwapChain() {
        auto extent = window->getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window->getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(logicalDevice);

        std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
        swapChain = std::make_unique<SwapChain>(device, extent, oldSwapChain);

        if (!oldSwapChain->compareFormats(*swapChain)) {
            throwEx("Swap chain image(or depth) format has changed!");
        }

        imgui->resize(*swapChain);
    }

} // namespace re