#ifndef RAVENENGINE_SWAPCHAIN_HPP
#define RAVENENGINE_SWAPCHAIN_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class Device;
    class Window;
    class Image;

    /**
     * @brief SwapChain Class. Manage the framebuffer and sync
     */
    class SwapChain : NonCopyable {
    public:
        /**
         * @brief Surface Support Details for creating SwapChain
         */
        struct SupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

    public:
        /**
         * @brief SwapChain constructor without recreating SwapChain
         * @param device shared_ptr of Device class
         * @param windowExtent Current window extent
         */
        SwapChain(const std::shared_ptr<Device>& device, VkExtent2D windowExtent);

        /**
         * @brief SwapChain Constructor recreating the SwapChain
         * @param device shared_ptr of Device class
         * @param windowExtent Current window extent
         * @param surface Window surface
         * @param oldSwapChain shared_ptr of SwapChain class for recreating it
         */
        SwapChain(const std::shared_ptr<Device>&  device, VkExtent2D windowExtent, std::shared_ptr<SwapChain>  oldSwapChain);

        ~SwapChain() override;

        /**
         *
         * @param index Current image index
         */
        [[nodiscard]] VkFramebuffer getFrameBuffer(uint32_t index) const;

        [[nodiscard]] VkRenderPass getRenderPass() const;

        /**
         *
         * @param index Current image index
         */
        [[nodiscard]] VkImageView getImageView(uint32_t index) const;

        [[nodiscard]] size_t getImageCount() const;

        [[nodiscard]] VkFormat getFormat() const;

        [[nodiscard]] VkExtent2D getExtent() const;

        /**
         *
         * @return Aspect ratio of SwapChain extent
         */
        [[nodiscard]] float getExtentAspectRatio() const;

        /**
         * @brief Acquire next SwapChain image
         * @param imageIndex Reference of uint32_t variable to save the current image index on it
         * @return Acquire next image operation result
         */
        VkResult acquireNextImage(uint32_t& imageIndex);

        /**
         * @brief Submit command buffer to the graphics queue
         * @param commandBuffer Command buffer with all recording commands
         * @param imageIndex Current image index
         * @return Submit command buffer operation result
         */
        VkResult submitCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        /**
         * @brief Check format and depth format of current old and new SwapChains
         * @param other Const reference of new SwapChain
         * @return bool value if old and new SwapChain have the same formats
         */
        [[nodiscard]] bool compareFormats(const SwapChain& other) const;

        /**
         * @brief Query the surface SupportDetails for the SwapChain
         * @param device Current physical device
         * @param surface Current window surface
         * @return SupportDetails with all the support details
         */
        static SupportDetails querySurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    private:
        /**
         * @brief Create all SwapChain resources
         */
        void init();

        /**
         * @brief Create SwapChain and its Images
         */
        void createSwapChain();

        /**
         * @brief Crete SwapChain images views
         */
        void createImageViews();

        /**
         * @brief Create Depth Images and ImageVies
         */
        void createDepthResources();

        /**
         * @brief Create the RenderPass with Color and Depth attachment
         */
        void createRenderPass();

        /**
         * @brief Create Framebuffers with Color and Depth attachments
         */
        void createFramebuffers();

        /**
         * @brief Create Semaphores and Fences for SwapChan sync
         */
        void createSyncObjects();

        /**
         * @brief Choose Surface Format with specific format and color space.\n\n
         * By default the format required is VK_FORMAT_B8G8R8A8_SRGB and color space VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
         * @param availableFormats Vector with all the available surface formats supported by the physical device
         * @return The Surface format with specific format and color space if exists. Else return the first format in the vector
         */
        static VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        /**
         * @brief Choose Present mode. The desired present mode is VK_PRESENT_MODE_MAILBOX_KHR, but if physical device
         * not support it, the engine use VK_PRESENT_MODE_FIFO_KHR
         * @param availablePresentModes Vector with all Present modes supported for the physical device
         * @return Desired Present mode if exists
         */
        static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        /**
         * Choose swapChain extent
         * @param capabilities Surface Capabilities supported for the Physical device
         * @return Required extent
         */
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
