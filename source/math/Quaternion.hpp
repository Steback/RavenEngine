#ifndef RAVENENGINE_QUATERNION_HPP
#define RAVENENGINE_QUATERNION_HPP


#include <limits>
#include <string>

#include "Vector3.hpp"
#include "Matrix3.hpp"


namespace re {

    class Quaternion {
    public:
        inline Quaternion() = default;

        inline Quaternion(float w, float x, float y, float z);

        inline explicit Quaternion(const Vector3& angles);

        explicit Quaternion(const float* v);

        explicit Quaternion(const double* v);

        inline Quaternion(const Quaternion& q) = default;

        [[nodiscard]] inline float lengthSqrt() const;

        [[nodiscard]] float length() const;

        [[nodiscard]] bool isUnit(float tol = std::numeric_limits<float>::epsilon()) const;

        void normalize();

        [[nodiscard]] Quaternion normalized() const;

        [[nodiscard]] inline float dot(const Quaternion& q) const;

        [[nodiscard]] inline Quaternion conjugate() const;

        void inverse();

        [[nodiscard]] Quaternion inversed() const;

        [[nodiscard]] inline Vector3 getAngles() const;

        [[nodiscard]] Matrix3 getMatrix() const;

        static Quaternion eulerAngles2QuatXZY(const Vector3& angles);

        static Quaternion eulerAngles2QuatXYZ(const Vector3& angles);

        static Quaternion eulerAngles2QuatYXZ(const Vector3& angles);

        static Quaternion eulerAngles2QuatYZX(const Vector3& angles);

        static Quaternion eulerAngles2QuatZYX(const Vector3& angles);

        static Quaternion eulerAngles2QuatZXY(const Vector3& angles);

        static Vector3 quat2EulerAnglesXZY(const Quaternion& quat);

        static Vector3 quat2EulerAnglesXYZ(const Quaternion& quat);

        static Vector3 quat2EulerAnglesYXZ(const Quaternion& quat);

        static Vector3 quat2EulerAnglesYZX(const Quaternion& quat);

        static Vector3 quat2EulerAnglesZYX(const Quaternion& quat);

        static Vector3 quat2EulerAnglesZXY(const Quaternion& quat);

        [[nodiscard]] std::string str() const;

        inline Quaternion& operator=(const Quaternion&) = default;

        inline Quaternion& operator=(Quaternion&&) = default;

        inline float& operator[](size_t i);

        inline const float& operator[](size_t i) const;

        inline Quaternion operator-() const;

        inline void operator+=(const Quaternion& q);

        inline void operator-=(const Quaternion& q);

        inline void operator*=(float s);

        inline void operator*=(const Quaternion& q);

        inline void operator/=(float s);

        inline Quaternion operator+(const Quaternion& q) const;

        inline Quaternion operator-(const Quaternion& q) const;

        inline Quaternion operator*(float s) const;

        inline Quaternion operator*(const Quaternion& q) const;

        inline Quaternion operator/(float s) const;

        inline bool operator==(const Quaternion& q) const;

        inline bool operator!=(const Quaternion& q) const;

        union {
            struct {
                float w, x, y, z;
            };
            float values[4]{};
        };
    };

    Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {

    }

    Quaternion::Quaternion(const Vector3 &angles) {
        *this = eulerAngles2QuatYZX(angles);
    }

    float Quaternion::lengthSqrt() const {
        return dot(*this);
    }

    inline Quaternion operator*(float n, const Quaternion& q) {
        return q * n;
    }

    float Quaternion::dot(const Quaternion &q) const {
        return w * q.w + x * q.x + y * q.y + z * q.z;
    }

    Quaternion Quaternion::conjugate() const {
        return {w, -x, -y, -z};
    }

    Vector3 Quaternion::getAngles() const {
        return quat2EulerAnglesYZX(*this);
    }

    float &Quaternion::operator[](size_t i) {
        return values[i];
    }

    const float &Quaternion::operator[](size_t i) const {
        return values[i];
    }

    Quaternion Quaternion::operator-() const {
        return {-w, -x, -y, -z};
    }

    void Quaternion::operator+=(const Quaternion &q) {
        *this = *this + q;
    }

    void Quaternion::operator-=(const Quaternion &q) {
        *this = *this - q;
    }

    void Quaternion::operator*=(float s) {
        *this = *this * s;
    }

    void Quaternion::operator*=(const Quaternion &q) {
        *this = *this * q;
    }

    void Quaternion::operator/=(float s) {
        *this = *this / s;
    }

    Quaternion Quaternion::operator+(const Quaternion &q) const {
        return {w + q.w, x + q.x, y + q.y, z + q.z};
    }

    Quaternion Quaternion::operator-(const Quaternion &q) const {
        return {w - q.w, x - q.x, y - q.y, z - q.z};
    }

    Quaternion Quaternion::operator*(float s) const {
        return {w * s, x * s, y * s, z * s};
    }

    Quaternion Quaternion::operator*(const Quaternion &q) const {
        return {
                w * q.w - x * q.x - y * q.y - z * q.z,
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + x * q.w
        };
    }

    Quaternion Quaternion::operator/(float s) const {
        return {w / s, x / s, y / s, z / s};
    }

    bool Quaternion::operator==(const Quaternion &q) const {
        return w == q.w && x == q.x && y == q.y && z == q.z;
    }

    bool Quaternion::operator!=(const Quaternion &q) const {
        return w != q.w && x != q.x && y != q.y && z != q.z;
    }

    using quat = Quaternion;

} // namespace re


#endif //RAVENENGINE_QUATERNION_HPP
