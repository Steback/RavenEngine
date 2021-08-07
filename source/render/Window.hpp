#ifndef RAVENENGINE_WINDOW_HPP
#define RAVENENGINE_WINDOW_HPP


#include <string>

#include "GLFW/glfw3.h"


namespace re {

    class Window {
    public:
        Window(std::string  name, int width, int height);

        ~Window();

        [[nodiscard]] bool isOpen() const;

    private:
        GLFWwindow* window{};
        std::string name;
        int width;
        int height;
    };

} // namespace re


#endif //RAVENENGINE_WINDOW_HPP
