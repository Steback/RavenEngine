#ifndef RAVENENGINE_SWAPCHAIN_HPP
#define RAVENENGINE_SWAPCHAIN_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "engine/core/NonCopyable.hpp"


namespace re {

    class Device;
    class Window;
    class Image;

    class SwapChain : NonCopyable {
    public:
        struct SupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

    public:
        SwapChain(const std::shared_ptr<Device>& device, VkExtent2D windowExtent);

        SwapChain(const std::shared_ptr<Device>&  device, VkExtent2D windowExtent, std::shared_ptr<SwapChain>  oldSwapChain);

        ~SwapChain() override;

        [[nodiscard]] VkFramebuffer getFrameBuffer(uint32_t index) const;

        [[nodiscard]] VkRenderPass getRenderPass() const;

        [[nodiscard]] VkImageView getImageView(uint32_t index) const;

        [[nodiscard]] size_t getImageCount() const;

        [[nodiscard]] VkFormat getFormat() const;

        [[nodiscard]] VkExtent2D getExtent() const;

        [[nodiscard]] float getExtentAspectRatio() const;

        VkResult acquireNextImage(uint32_t& imageIndex);

        VkResult submitCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        [[nodiscard]] bool compareFormats(const SwapChain& other) const;

        static SupportDetails querySurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    private:
        void init();

        void createSwapChain();

        void createImageViews();

        void createDepthResources();

        void createRenderPass();

        void createFramebuffers();

        void createSyncObjects();

        static VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    private:
        std::shared_ptr<Device> device{};
        VkDevice logicalDevice{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};

        VkSwapchainKHR swapChain{};
        std::shared_ptr<SwapChain> oldSwapChain{};

        VkFormat format{};
        VkFormat depthFormat{};

        VkExtent2D extent{};
        VkExtent2D windowExtent{};

        std::vector<VkFramebuffer> framebuffers;
        VkRenderPass renderPass{};

        std::vector<std::unique_ptr<Image>> images;
        std::vector<std::unique_ptr<Image>> depthImages;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame{0};
    };

} // namespace re


#endif //RAVENENGINE_SWAPCHAIN_HPP
