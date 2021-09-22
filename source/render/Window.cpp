#include "Window.hpp"

#include "utils/Utils.hpp"


namespace re {

    /**
     * @brief Setup GLFW and create Window
     * @param name Window name
     * @param width Window width
     * @param height Window height
     */
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

    /**
     * @brief Check if windows is currently open
     */
    bool Window::isOpen() const {
        return !glfwWindowShouldClose(window);
    }

    /**
     * @brief Create vulkan surface for this window
     * @param instance Pointer to Instance
     * @param surface Pointer to vulkan surface
     */
    void Window::createSurface(VkInstance instance, VkSurfaceKHR *surface) {
        checkResult(glfwCreateWindowSurface(instance, window, nullptr, surface),
                    "Failed to create window surface");
    }

    /**
     *
     * @return Current window extent
     */
    VkExtent2D Window::getExtent() const {
        return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    /**
     *
     * @return Check if windows was resized
     */
    bool Window::wasWindowResized() const {
        return framebufferResized;
    }

    /**
     *
     * @return Raw GLFW Window
     */
    GLFWwindow *Window::getHandle() const {
        return window;
    }

    /**
     * @brief Reset windows was resized flag to false
     */
    void Window::resetWindowResizedFlag() {
        framebufferResized = false;
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->framebufferResized = true;
        win->width = width;
        win->height = height;
    }

} // namespace re