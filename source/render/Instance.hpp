#ifndef RAVENENGINE_INSTANCE_HPP
#define RAVENENGINE_INSTANCE_HPP


#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "utils/Macros.hpp"


namespace re {

    /**
     * @brief Vulkan Instance wrapper
     */
    class Instance : NonCopyable {
    public:
        /**
         * @brief Default constructor
         * @param Application/Game name
         * @param layers vector of C-Style strings with all required layers
         */
        Instance(const char* appName, const std::vector<const char*>& layers);

        ~Instance() override;

        /**
         * @brief Find a Physical Device that have all required extensions and vulkan support
         * @param extensions vector of C-Style strings with all the extensions names
         * @return Physical Device if exists
         */
        VkPhysicalDevice pickPhysicalDevice(const std::vector<const char*>& extensions);

        /**
         * @brief Check if the Physical Device have all the required extensions
         * @param device Vulkan physical device
         * @param extensions vector of C-Style strings with all the extensions names
         * @return bool value, if the Physical Device support the extensions
         */
        static bool checkExtensionsSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);

        /**
         *
         * @return Vulkan raw Instance
         */
        [[nodiscard]] VkInstance getInstance() const;

    private:
        /**
         * @brief Get all needed extensions by GLFW for create the instance
         * @return vector of C-Style strings with all extensions names
         */
        static std::vector<const char*> getRequiredExtensions();

        /**
         * @brief Check if the required layer are available in the computer
         * @param layers vector of C-Style strings with the layers names
         * @return bool value, if all the layers are available or not
         */
        static bool checkLayersSupport(const std::vector<const char*>& layers);

    private:
        VkInstance instance{};
#ifdef RE_DEBUG
        VkDebugUtilsMessengerEXT debugMessenger{};
#endif
    };

} // namespace re


#endif //RAVENENGINE_INSTANCE_HPP
