#ifndef RAVENENGINE_RENDERER_HPP
#define RAVENENGINE_RENDERER_HPP


#include <string>
#include <memory>


namespace re {

    class Window;

    class Renderer {
    public:
        Renderer(const std::string& appName);

        ~Renderer();

        [[nodiscard]] bool isWindowOpen() const;

    private:
        std::shared_ptr<Window> window;
    };

} // namespace re


#endif //RAVENENGINE_RENDERER_HPP
