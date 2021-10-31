#ifndef RAVENENGINE_RENDERER_HPP
#define RAVENENGINE_RENDERER_HPP


#include <vector>
#include <string>
#include <memory>

#include "vulkan/vulkan.h"

#include "engine/core/NonCopyable.hpp"


namespace re {

    namespace ui {
        class ImGuiRender;
    }

    class Window;
    class Config;
    class Instance;
    class Device;
    class SwapChain;
    class Vector2;

    class Renderer : NonCopyable {
    public:
        Renderer(const std::string& appName, const Config& config);

        ~Renderer() override;

        [[nodiscard]] bool isWindowOpen() const;

        [[nodiscard]] VkRenderPass getRenderPass() const;

        [[nodiscard]] bool isFrameInProgress() const;

        [[nodiscard]] VkCommandBuffer getCommandBuffer() const;

        [[nodiscard]] int getFrameIndex() const;

        VkCommandBuffer beginFrame();

        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

        void waitDeviceIde();

        [[nodiscard]] std::shared_ptr<Device> getDevice() const;

        [[nodiscard]] float getAspectRatio() const;

        void newImGuiFrame();

        void renderImGui(VkCommandBuffer commandBuffer);

        [[nodiscard]] uint32_t getImageCount() const;

        Vector2 getWindowSize() const;

    private:
        void createCommandBuffers();

        void freeCommandBuffers();

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
