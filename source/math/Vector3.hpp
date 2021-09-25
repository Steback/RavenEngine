#ifndef RAVENENGINE_VECTOR3_HPP
#define RAVENENGINE_VECTOR3_HPP


#include <string>


namespace re {

    class Vector3 {
    public:
        Vector3();

        explicit Vector3(float n);

        Vector3(float x,float y, float z);

        explicit Vector3(const float* p);

        explicit Vector3(const double * p);

        Vector3(const Vector3& v);

        Vector3& operator=(const Vector3& v);

        float& operator[](unsigned int index);

        const float& operator[](unsigned int index) const;

        bool operator==(const Vector3& v) const;

        bool operator!=(const Vector3& v) const;

        Vector3 operator-() const;

        Vector3 operator+(float n) const;

        Vector3 operator+(const Vector3& v) const;

        Vector3 operator-(float n) const;

        Vector3 operator-(const Vector3& v) const;

        Vector3 operator*(float n) const;

        float operator*(const Vector3& v) const;

        Vector3 operator/(float n) const;

        Vector3& operator+=(float n);

        Vector3& operator+=(const Vector3& v);

        Vector3& operator-=(float n);

        Vector3& operator-=(const Vector3& v);

        Vector3& operator*=(float n);

        Vector3& operator/=(float n);

        [[nodiscard]] float lengthSqrt() const;

        [[nodiscard]] float length() const;

        [[nodiscard]] Vector3 normal() const;

        Vector3& normalize();

        float* ptr();

        [[nodiscard]] std::string str() const;

        [[nodiscard]] Vector3 cross(const Vector3& v) const;

    public:
        float x{}, y{}, z{};
    };

    inline Vector3 operator+(float n, const Vector3& v) {
        return v + n;
    }

    inline Vector3 operator-(float n, const Vector3& v) {
        return v - n;
    }

    inline Vector3 operator*(float n, const Vector3& v) {
        return v * n;
    }

    inline Vector3 operator/(float n, const Vector3& v) {
        return {n / v.x, n / v.y, n / v.z};
    }

    using vec3 = Vector3;

} // namespace re


#endif //RAVENENGINE_VECTOR3_HPP
