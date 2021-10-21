#ifndef RAVENENGINE_MATH_HPP
#define RAVENENGINE_MATH_HPP


#include <cmath>

#include "Typedefs.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "engine/utils/Typedefs.hpp"


namespace re {

    class Math {
    public:
        static inline float pi() { return Math_PI; }

        static inline float twoPi() { return Math_PI * 2; }

        static inline float sin(float x) { return ::sinf(x); }
        static inline Vector3 sin(const Vector3& v) { return { sin(v.x), sin(v.y), sin(v.z)}; }

        static inline float cos(float x) { return ::cosf(x); }
        static inline Vector3 cos(const Vector3& v) { return { cos(v.x), cos(v.y), cos(v.z)}; }

        static inline float tan(float x) { return ::tanf(x); }

        static inline float asin(float x) { return ::asinf(x); }

        static inline float acos(float x) { return ::acosf(x); }

        static inline float atan(float x) { return ::atanf(x); }

        static inline float atan2(float y, float x) { return ::atan2f(y, x); }

        static inline float sqrt(float x) { return ::sqrtf(x); }

        static inline float fmod(float x, float y) { return ::fmodf(x, y); }

        static inline float floor(float x) { return ::floorf(x); }

        static inline float ceil(float x) { return ::ceilf(x); }

        static inline float pow(float x, float y) { return ::powf(x, y); }

        static inline float log(float x) { return ::logf(x); }

        static inline float log2(float x) { return ::log2f(x); }

        static inline float exp(float x) { return ::expf(x); }

        static inline int abs(int x) { return x > 0 ? x : -x; }
        static inline float abs(float x) { return ::abs(x); }

        static inline float deg2rad(float x) { return x * (Math_PI / 180.0); }
        static inline Vector3 deg2rad(const Vector3& v) { return {deg2rad(v.x), deg2rad(v.y), deg2rad(v.z)}; }

        static inline float rad2deg(float x) { return x * (180.0 / Math_PI); }
        static inline Vector3 rad2deg(const Vector3& v) { return {rad2deg(v.x), rad2deg(v.y), rad2deg(v.z)}; }
    };

} // namespace re


#endif //RAVENENGINE_MATH_HPP
