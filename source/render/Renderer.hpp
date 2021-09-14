#ifndef RAVENENGINE_RENDERER_HPP
#define RAVENENGINE_RENDERER_HPP


#include <vector>
#include <string>
#include <memory>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace re {

    namespace ui {
        class ImGuiRender;
    }

    class Window;
    class Config;
    class Instance;
    class Device;
    class SwapChain;

    /**
     * @brief Main render class, execute all render commands with Vulkan API
     */
    class Renderer : NonCopyable {
    public:
        /**
         *
         * @param appName Application/Game name
         */
        Renderer(const std::string& appName, const Config& config);

        ~Renderer() override;

        /**
         *
         * @return bool value that depend if main windows is currently open or not
         */
        [[nodiscard]] bool isWindowOpen() const;

        /**
         *
         * @return SwapChain Render pass
         */
        [[nodiscard]] VkRenderPass getRenderPass() const;

        /**
         *
         * @return Current frame is still in progress or not
         */
        [[nodiscard]] bool isFrameInProgress() const;

        /**
         *
         * @return Current command uboBuffer in recording state
         */
        [[nodiscard]] VkCommandBuffer getCommandBuffer() const;

        /**
         *
         * @return Current frame index
         */
        [[nodiscard]] int getFrameIndex() const;

        /**
         * Begin frame for recording the render commands
         * @return Current command uboBuffer in recording state
         */
        VkCommandBuffer beginFrame();

        /**
         * End current frame and submit the commands
         */
        void endFrame();

        /**
         * Set clear color values, viewport and Scissor;
         * @param commandBuffer Command uboBuffer in recording state
         */
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        /**
         * End Render Pass
         * @param commandBuffer Command uboBuffer in recording state
         */
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

        /**
         * @brief Wait until the logical device is idle
         */
        void waitDeviceIde();

        /**
         *
         * @return shared_ptr of Device class
         */
        [[nodiscard]] std::shared_ptr<Device> getDevice() const;

        /**
         *
         * @return Aspect ratio of SwapChain extent
         */
        [[nodiscard]] float getAspectRatio() const;

        /**
         * @brief Begin new ImGui frame
         */
        void newImGuiFrame();

        /**
         *
         * @param commandBuffer Valid command uboBuffer in recording state
         */
        void renderImGui(VkCommandBuffer commandBuffer);

        [[nodiscard]] uint32_t getImageCount() const;

    private:
        /**
         * @brief Create command buffers
         */
        void createCommandBuffers();

        /**
         * @brief Free command buffers
         */
        void freeCommandBuffers();

        /**
         * @brief Recreate SwapChain and check the old and new SwapChains have the same formats
         */
        void recreateSwapChain();

    private:
        VkDevice logicalDevice{};

        std::shared_ptr<Window> window;
        std::shared_ptr<Instance> instance;
        std::shared_ptr<Device> device;

        std::unique_ptr<SwapChain> swapChain;

        VkCommandPool commandPool{};
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t imageIndex{};
        int currentFrameIndex{0};
        bool isFrameStarted{false};

        std::shared_ptr<ui::ImGuiRender> imgui;
    };

} // namespace re


#endif //RAVENENGINE_RENDERER_HPP
