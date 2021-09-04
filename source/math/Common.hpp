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
     */
    inline float radians(float angle) {
        return angle * 0.01745329251994329576923690768489f;
    }

    /**
     *
     * @param angles Vector3 of angles in degrees
     */
    inline Vector3 radians(const Vector3& angles) {
        return angles * 0.01745329251994329576923690768489f;
    }

    /**
     *
     * @param angle Angles in radians
     */
    inline float degrees(float angle) {
        return angle * 57.295779513082320876798154814105f;
    }

    /**
     *
     * @param angles Vector3 of angles in radians
     */
    inline Vector3 degrees(const Vector3& angles) {
        return angles * 57.295779513082320876798154814105f;
    }
}


#endif //LITTLEVULKANENGINE_COMMON_HPP
