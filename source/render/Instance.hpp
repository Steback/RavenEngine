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

    private:
        /**
         * @brief Get all needed extensions by GLFW for create the instance
         * @return vector of C-Style strings with all extensions names
         */
        static std::vector<const char*> getRequiredExtensions();

    private:
        VkInstance instance{};
#ifdef RE_DEBUG
        VkDebugUtilsMessengerEXT debugMessenger{};
#endif
    };

} // namespace re


#endif //RAVENENGINE_INSTANCE_HPP
