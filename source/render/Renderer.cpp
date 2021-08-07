#include "Renderer.hpp"

#include "Window.hpp"


namespace re {

    Renderer::Renderer(const eastl::string& appName) {
        window = eastl::make_shared<Window>(appName, 800, 600);
    }

    Renderer::~Renderer() = default;

    bool Renderer::isWindowOpen() const {
        return window->isOpen();
    }

} // namespace re