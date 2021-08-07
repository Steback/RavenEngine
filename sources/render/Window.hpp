#ifndef RAVENENGINE_WINDOW_HPP
#define RAVENENGINE_WINDOW_HPP


#include "GLFW/glfw3.h"
#include "EASTL/string.h"


namespace re {

    class Window {
    public:
        Window(eastl::string  name, int width, int height);

        ~Window();

        [[nodiscard]] bool isOpen() const;

    private:
        GLFWwindow* window{};
        eastl::string name;
        int width;
        int height;
    };

} // namespace re


#endif //RAVENENGINE_WINDOW_HPP
