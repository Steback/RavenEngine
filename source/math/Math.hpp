#ifndef RAVENENGINE_MATH_HPP
#define RAVENENGINE_MATH_HPP


#include <cmath>

#include "Typedefs.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "utils/Typedefs.hpp"


namespace re {

    class Math {
    public:
        static _ALWAYS_INLINE_ float pi() { return Math_PI; }

        static _ALWAYS_INLINE_ float twoPi() { return Math_PI * 2; }

        static _ALWAYS_INLINE_ float sin(float x) { return ::sinf(x); }
        static _ALWAYS_INLINE_ Vector3 sin(const Vector3& v) { return { sin(v.x), sin(v.y), sin(v.z)}; }

        static _ALWAYS_INLINE_ float cos(float x) { return ::cosf(x); }
        static _ALWAYS_INLINE_ Vector3 cos(const Vector3& v) { return { cos(v.x), cos(v.y), cos(v.z)}; }

        static _ALWAYS_INLINE_ float tan(float x) { return ::tanf(x); }

        static _ALWAYS_INLINE_ float asin(float x) { return ::asinf(x); }

        static _ALWAYS_INLINE_ float acos(float x) { return ::acosf(x); }

        static _ALWAYS_INLINE_ float atan(float x) { return ::atanf(x); }

        static _ALWAYS_INLINE_ float atan2(float y, float x) { return ::atan2f(y, x); }

        static _ALWAYS_INLINE_ float sqrt(float x) { return ::sqrtf(x); }

        static _ALWAYS_INLINE_ float fmod(float x, float y) { return ::fmodf(x, y); }

        static _ALWAYS_INLINE_ float floor(float x) { return ::floorf(x); }

        static _ALWAYS_INLINE_ float ceil(float x) { return ::ceilf(x); }

        static _ALWAYS_INLINE_ float pow(float x, float y) { return ::powf(x, y); }

        static _ALWAYS_INLINE_ float log(float x) { return ::logf(x); }

        static _ALWAYS_INLINE_ float log2(float x) { return ::log2f(x); }

        static _ALWAYS_INLINE_ float exp(float x) { return ::expf(x); }

        static _ALWAYS_INLINE_ int abs(int x) { return x > 0 ? x : -x; }
        static _ALWAYS_INLINE_ float abs(float x) { return ::abs(x); }

        static _ALWAYS_INLINE_ float deg2rad(float x) { return x * (Math_PI / 180.0); }
        static _ALWAYS_INLINE_ Vector3 deg2rad(const Vector3& v) { return {deg2rad(v.x), deg2rad(v.y), deg2rad(v.z)}; }

        static _ALWAYS_INLINE_ float rad2deg(float x) { return x * (180.0 / Math_PI); }
        static _ALWAYS_INLINE_ Vector3 rad2deg(const Vector3& v) { return {rad2deg(v.x), rad2deg(v.y), rad2deg(v.z)}; }
    };

} // namespace re


#endif //RAVENENGINE_MATH_HPP
