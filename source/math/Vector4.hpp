#ifndef RAVENENGINE_VECTOR4_HPP
#define RAVENENGINE_VECTOR4_HPP


#include <string>


namespace re {

    class Vector4 {
    public:
        Vector4();

        explicit Vector4(float n);

        Vector4(float x, float y, float z, float w);

        explicit Vector4(const float* p);

        explicit Vector4(const double * p);

        Vector4(const Vector4& v);

        Vector4& operator=(const Vector4& v);

        float& operator[](unsigned int index);

        const float& operator[](unsigned int index) const;

        bool operator==(const Vector4& v) const;

        bool operator!=(const Vector4& v) const;

        Vector4 operator-() const;

        Vector4 operator+(float n) const;

        Vector4 operator+(const Vector4& v) const;

        Vector4 operator-(float n) const;

        Vector4 operator-(const Vector4& v) const;

        Vector4 operator*(float n) const;

        float operator*(const Vector4& v) const;

        Vector4 operator/(float n) const;

        Vector4& operator+=(float n);

        Vector4& operator+=(const Vector4& v);

        Vector4& operator-=(float n);

        Vector4& operator-=(const Vector4& v);

        Vector4& operator*=(float n);

        Vector4& operator/=(float n);

        [[nodiscard]] float lengthSqrt() const;

        [[nodiscard]] float length() const;

        [[nodiscard]] Vector4 normal() const;

        Vector4& normalize();

        float* ptr();

        [[nodiscard]] std::string str() const;

    public:
        float x{}, y{}, z{}, w{};
    };

    inline Vector4 operator+(float n,  const Vector4& v) {
        return v + n;
    }

    inline Vector4 operator-(float n,  const Vector4& v) {
        return v - n;
    }

    inline Vector4 operator*(float n,  const Vector4& v) {
        return v * n;
    }

    using vec4 = Vector4;


} // namespace re


#endif //RAVENENGINE_VECTOR4_HPP
