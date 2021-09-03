#ifndef LITTLEVULKANENGINE_COMMON_HPP
#define LITTLEVULKANENGINE_COMMON_HPP


#include <cmath>

#include "Vector3.hpp"


namespace re {

    /**
     *
     * @return Number PI
     */
    inline float pi() {
        return 3.14159265358979323846264338327950288;
    }

    /**
     *
     * @return 2 * PI or 360 degrees
     */
    inline float twoPi() {
        return 2 * pi();
    }

    /**
     *
     * @param a
     * @param b
     * @return Modulus of two float numbers
     */
    inline float mod(float a, float b) {
        return a - b * std::floor(a / b);
    }

    /**
     *
     * @param angle Angles in degrees
     * @return Angle in radians
     */
    inline float radians(float angle) {
        return angle * 0.01745329251994329576923690768489f;
    }

    inline Vector3 radians(const Vector3& angles) {
        return {radians(angles.x), radians(angles.y), radians(angles.z)};
    }

}


#endif //LITTLEVULKANENGINE_COMMON_HPP
