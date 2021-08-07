#include "Window.hpp"

#include "utils/Macros.hpp"


namespace re {

    Window::Window(eastl::string name, int width, int height) : name(std::move(name)), width(width), height(height) {
        if (!glfwInit()) RE_THROW_EX("Failed to init GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool Window::isOpen() const {
        return !glfwWindowShouldClose(window);
    }

} // namespace re