#ifndef RAVENENGINE_INSTANCE_HPP
#define RAVENENGINE_INSTANCE_HPP


#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "utils/Macros.hpp"


namespace re {

    class Instance : NonCopyable {
    public:
        Instance(const char* appName, const std::vector<const char*>& layers);

        ~Instance() override;

        VkPhysicalDevice pickPhysicalDevice(const std::vector<const char*>& extensions);

        static bool checkExtensionsSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);

        [[nodiscard]] VkInstance getInstance() const;

    private:
        static std::vector<const char*> getRequiredExtensions();

        static bool checkLayersSupport(const std::vector<const char*>& layers);

    private:
        VkInstance instance{};
#ifdef RE_DEBUG
        VkDebugUtilsMessengerEXT debugMessenger{};
#endif
    };

} // namespace re


#endif //RAVENENGINE_INSTANCE_HPP
