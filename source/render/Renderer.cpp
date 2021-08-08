#include "Renderer.hpp"

#include "Window.hpp"
#include "config/Config.hpp"


namespace re {

    Renderer::Renderer(const std::string& appName, const Config& config) {
        window = std::make_shared<Window>(appName, config.getWidth(), config.getHeight());
    }

    Renderer::~Renderer() = default;

    bool Renderer::isWindowOpen() const {
        return window->isOpen();
    }

} // namespace re