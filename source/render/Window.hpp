#ifndef RAVENENGINE_WINDOW_HPP
#define RAVENENGINE_WINDOW_HPP


#include <string>

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

    private:
        GLFWwindow* window{};
        std::string name;
        int width;
        int height;
    };

} // namespace re


#endif //RAVENENGINE_WINDOW_HPP
