#include "Renderer.hpp"

#include "Window.hpp"
#include "Instance.hpp"
#include "config/Config.hpp"


namespace re {

    Renderer::Renderer(const std::string& appName, const Config& config) {
        window = std::make_shared<Window>(appName, config.getWidth(), config.getHeight());

        std::vector<const char *> validationLayers;

#ifdef RE_DEBUG
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(appName.c_str(), validationLayers);
    }

    Renderer::~Renderer() = default;

    bool Renderer::isWindowOpen() const {
        return window->isOpen();
    }

} // namespace re