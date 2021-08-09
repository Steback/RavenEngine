#ifndef RAVENENGINE_WINDOW_HPP
#define RAVENENGINE_WINDOW_HPP


#include <string>

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"


namespace re {

    /**
     * @brief Class with GLFW functionalities for window management
     */
    class Window {
    public:
        /**
         *
         * @param name Window name
         * @param width Window width
         * @param height Window height
         */
        Window(std::string  name, int width, int height);

        ~Window();

        /**
         *
         * @return bool value, Window is currently open or not
         */
        [[nodiscard]] bool isOpen() const;

        /**
         * Create vulkan surface for window
         * @param instance Raw vulkan instance
         * @param surface Pointer to vulkan surface
         */
        void createSurface(VkInstance instance, VkSurfaceKHR *surface);

        /**
         *
         * @return Vulkan extent 2D with the window resolution
         */
        [[nodiscard]] VkExtent2D getExtent() const;

        /**
         *
         * @return bool value that depends if windows was resized or not
         */
        [[nodiscard]] bool wasWindowResized() const;

        /**
         * @brief Once the Window was resized change the value of the flag to false
         */
        void resetWindowResizedFlag();

    private:
        /**
         * @brief Window resize callback to GLFW
         * @param window GLFW window pointer
         * @param width New window width
         * @param height New window height
         */
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
