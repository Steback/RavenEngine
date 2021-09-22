#ifndef RAVENENGINE_MATRIX4_HPP
#define RAVENENGINE_MATRIX4_HPP


#include <string>

#include "Vector4.hpp"


namespace re {

    class Matrix4 {
    public:
        Matrix4();

        explicit Matrix4(float n);

        Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);

        explicit Matrix4(const Vector4 m[4]);

        explicit Matrix4(const float* p);

        explicit Matrix4(const double * p);

        Matrix4(const Matrix4& m);

        Matrix4& operator=(const Matrix4& m);

        Vector4& operator[](size_t row);

        const Vector4& operator[](size_t row) const;

        bool operator==(const Matrix4& m) const;

        bool operator!=(const Matrix4& m) const;

        Matrix4 operator-() const;

        Matrix4 operator+(const Matrix4& m) const;

        Matrix4 operator-(const Matrix4& m) const;

        Matrix4 operator*(float n) const;

        Vector4 operator*(const Vector4& v) const;

        Matrix4 operator*(const Matrix4& m) const;

        Matrix4 operator/(float n) const;

        Matrix4& operator+=(const Matrix4& m);

        Matrix4& operator-=(const Matrix4& m);

        Matrix4& operator*=(float n);

        Matrix4& operator*=(const Matrix4& m);

        Matrix4& operator/=(float n);

        [[nodiscard]] float determinant() const;

        [[nodiscard]] Matrix4 adjugate() const;

        [[nodiscard]] Matrix4 inverse() const;

        [[nodiscard]] Matrix4 transpose() const;

        [[nodiscard]] std::string str(bool newLine = true) const;

    private:
        Vector4 values[4];
    };

    inline Matrix4 operator*(float n, const Matrix4& m) {
        return m * n;
    }

    inline Matrix4 operator/(float n, const Matrix4& m) {
        return m / n;
    }

    using mat4 = Matrix4;


} // namespace re


#endif //RAVENENGINE_MATRIX4_HPP
