#include "Window.hpp"

#include "utils/Macros.hpp"


namespace re {

    Window::Window(std::string name, int width, int height) : name(std::move(name)), width(width), height(height) {
        if (!glfwInit()) RE_THROW_EX("Failed to init GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool Window::isOpen() const {
        return !glfwWindowShouldClose(window);
    }

    void Window::createSurface(VkInstance instance, VkSurfaceKHR *surface) {
        RE_VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, nullptr, surface),
                           "Failed to create window surface");
    }

    VkExtent2D Window::getExtent() const {
        return {CAST_U32(width), CAST_U32(height)};
    }

    bool Window::wasWindowResized() const {
        return framebufferResized;
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->framebufferResized = true;
        win->width = width;
        win->height = height;
    }

    void Window::resetWindowResizedFlag() {
        framebufferResized = false;
    }

} // namespace re