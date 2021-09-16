#ifndef LITTLEVULKANENGINE_UTILS_HPP
#define LITTLEVULKANENGINE_UTILS_HPP


#include <functional>
#include <stdexcept>
#include <string>

#include "vulkan/vulkan_core.h"

#include "Macros.hpp"


namespace re {

    // from: https://stackoverflow.com/a/57595105
    template <typename T, typename... Rest>
    void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (hashCombine(seed, rest), ...);
    };

    /**
     * @brief Default throw exception macro
     */
    inline void throwEx(const std::string& message) {
        throw std::runtime_error(message);
    }

    /**
     * @brief Default Vulkan result check.
     *
     * If result isn't success throw a exception
     */
     inline void checkResult(VkResult result, const std::string& message) {
         if (result != VK_SUCCESS) throwEx(message);
     }

} // namespace re


#endif //LITTLEVULKANENGINE_UTILS_HPP
