#include "Window.hpp"

#include "utils/Utils.hpp"


namespace re {

    Window::Window(std::string name, int width, int height) : name(std::move(name)), width(width), height(height) {
        if (!glfwInit()) throwEx("Failed to init GLFW");

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
        checkResult(glfwCreateWindowSurface(instance, window, nullptr, surface),
                    "Failed to create window surface");
    }

    VkExtent2D Window::getExtent() const {
        return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    bool Window::wasWindowResized() const {
        return framebufferResized;
    }

    GLFWwindow *Window::getHandle() const {
        return window;
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