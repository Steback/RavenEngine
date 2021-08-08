#ifndef RAVENENGINE_MACROS_HPP
#define RAVENENGINE_MACROS_HPP


#include <stdexcept>


#if !defined(NDEBUG)
#define RE_DEBUG
#endif

/**
 * @brief Default throw exception macro
 */
#define RE_THROW_EX(message) throw std::runtime_error(message);

/**
 * @brief Cast to uint32_t type
 */
#define CAST_U32(num) static_cast<uint32_t>(num)

/**
 * @brief Default Vulkan result check.
 *
 * If result isn't success throw a exception
 */
#define RE_VK_CHECK_RESULT(result, message) if (result != VK_SUCCESS) RE_THROW_EX(message)


#endif //RAVENENGINE_MACROS_HPP
