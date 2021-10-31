#ifndef RAVENENGINE_IMGUIRENDER_HPP
#define RAVENENGINE_IMGUIRENDER_HPP


#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "engine/core/NonCopyable.hpp"


namespace re {

    class Device;
    class SwapChain;
    class Window;

    namespace ui {

        class ImGuiRender : NonCopyable {
        public:
            ImGuiRender(const std::shared_ptr<Device>& device, const SwapChain& swapChain, const std::shared_ptr<Window>& window);

            ~ImGuiRender() override;

            void resize(const SwapChain& swapChain);

            void newFrame();

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
