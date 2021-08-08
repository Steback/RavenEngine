#include "Renderer.hpp"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "config/Config.hpp"


namespace re {

    Renderer::Renderer(const std::string& appName, const Config& config) {
        window = std::make_shared<Window>(appName, config.getWidth(), config.getHeight());

        std::vector<const char *> validationLayers;

#ifdef RE_DEBUG
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(appName.c_str(), validationLayers);

        VkPhysicalDeviceFeatures features{};
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        device = std::make_shared<Device>(instance, extensions, features);
    }

    Renderer::~Renderer() = default;

    bool Renderer::isWindowOpen() const {
        return window->isOpen();
    }

} // namespace re