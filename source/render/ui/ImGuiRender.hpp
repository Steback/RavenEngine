#ifndef RAVENENGINE_IMGUIRENDER_HPP
#define RAVENENGINE_IMGUIRENDER_HPP


#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class Device;
    class SwapChain;
    class Window;

    namespace ui {

        // TODO: Change ImGui implementation to use Vulkan Memory Allocator
        /**
         * @brief ImGuiRender wrapper class
         */
        class ImGuiRender : NonCopyable {
        public:
            /**
             *
             * @param device Pointer to Device
             * @param swapChain Reference to valid SwapChain
             * @param window Pointer to current Window
             */
            ImGuiRender(const std::shared_ptr<Device>& device, const SwapChain& swapChain, const std::shared_ptr<Window>& window);

            ~ImGuiRender() override;

            /**
             *
             * @param swapChain
             */
            void resize(const SwapChain& swapChain);

            /**
             * @brief New ImGui frame
             */
            void newFrame();

            /**
             * @brief Render ImGui Data
             * @param commandBuffer Valid command uboBuffer in recording state
             */
            void render(VkCommandBuffer commandBuffer);

        private:
            void createDescriptorPool();

        private:
            VkDevice logicalDevice{};
            VkDescriptorPool descriptorPool{};
        };

    } // namespace ui

} // namespace re


#endif //RAVENENGINE_IMGUIRENDER_HPP
