#ifndef LITTLEVULKANENGINE_COMMON_HPP
#define LITTLEVULKANENGINE_COMMON_HPP


#include <cmath>

#include "Vector3.hpp"


namespace re {

    inline float pi() {
        return 3.14159265358979323846264338327950288;
    }

    inline float twoPi() {
        return 2 * pi();
    }

    inline float mod(float a, float b) {
        return a - b * std::floor(a / b);
    }

    inline float radians(float angle) {
        return angle * 0.01745329251994329576923690768489f;
    }
    inline Vector3 radians(const Vector3& angles) {
        return angles * 0.01745329251994329576923690768489f;
    }

    inline float degrees(float angle) {
        return angle * 57.295779513082320876798154814105f;
    }

    inline Vector3 degrees(const Vector3& angles) {
        return angles * 57.295779513082320876798154814105f;
    }

    inline Vector3 sin(const Vector3& angles) {
        return {std::sin(angles.x), std::sin(angles.y), std::sin(angles.z)};
    }

    inline Vector3 cos(const Vector3& angles) {
        return {std::cos(angles.x), std::cos(angles.y), std::cos(angles.z)};
    }
}


#endif //LITTLEVULKANENGINE_COMMON_HPP
