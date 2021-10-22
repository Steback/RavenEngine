#ifndef RAVENENGINE_WINDOW_HPP
#define RAVENENGINE_WINDOW_HPP


#include <string>

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"


namespace re {

    class Vector2;

    class Window {
    public:
        Window(std::string  name, int width, int height);

        ~Window();

        [[nodiscard]] bool isOpen() const;

        void createSurface(VkInstance instance, VkSurfaceKHR *surface);

        [[nodiscard]] VkExtent2D getExtent() const;

        [[nodiscard]] bool wasWindowResized() const;

        void resetWindowResizedFlag();

        [[nodiscard]] GLFWwindow* getHandle() const;

        [[nodiscard]] Vector2 getSize() const;

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

    private:
        GLFWwindow* window{};
        std::string name;
        int width;
        int height;
        bool framebufferResized{false};
    };

} // namespace re


#endif //RAVENENGINE_WINDOW_HPP
