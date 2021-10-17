#ifndef RAVENENGINE_MATRIX4_HPP
#define RAVENENGINE_MATRIX4_HPP


#include <string>

#include "Vector4.hpp"


namespace re {

    class Matrix4 {
    public:
        inline Matrix4() = default;

        inline explicit Matrix4(float n);

        inline Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w);

        inline Matrix4(const Matrix4& m) = default;

        void invert();

        [[nodiscard]] Matrix4 inverted() const;

        [[nodiscard]] Matrix4 adjugate() const;

        inline void transpose();

        inline Matrix4 transposed();

        [[nodiscard]] inline float determinant() const;

        inline void setIdentity();

        inline void setZero();

        inline void setDiagonal(const Vector4& dig);

        [[nodiscard]] std::string str() const;

        inline Matrix4& operator=(const Matrix4&) = default;

        inline Matrix4& operator=(Matrix4&&) = default;

        inline Vector4& operator[](size_t row);

        inline const Vector4& operator[](size_t row) const;

        inline Matrix4 operator-() const;

        inline void operator+=(const Matrix4& m);

        inline void operator-=(const Matrix4& m);

        inline void operator*=(float n);

        inline void operator*=(const Matrix4& m);

        inline void operator/=(float n);

        inline Matrix4 operator+(const Matrix4& m) const;

        inline Matrix4 operator-(const Matrix4& m) const;

        inline Matrix4 operator*(float n) const;

        inline Vector4 operator*(const Vector4& v) const;

        Matrix4 operator*(const Matrix4& m) const;

        inline Matrix4 operator/(float n) const;

        inline bool operator==(const Matrix4& m) const;

        inline bool operator!=(const Matrix4& m) const;

        Vector4 elements[4] = {
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
        };
    };

    Matrix4::Matrix4(float n) {
        elements[0] = {n, 0.0f, 0.0f, 0.0f};
        elements[1] = {0.0f, n, 0.0f, 0.0f};
        elements[2] = {0.0f, 0.0f, n, 0.0f};
        elements[3] = {0.0f, 0.0f, 0.0f, n};
    }

    Matrix4::Matrix4(const Vector4 &x, const Vector4 &y, const Vector4 &z, const Vector4 &w) {
        elements[0] = x;
        elements[1] = y;
        elements[2] = z;
        elements[3] = w;
    }

    void Matrix4::transpose() {
        *this = transposed();
    }

    Matrix4 Matrix4::transposed() {
        return {
            {elements[0][0], elements[1][0], elements[2][0], elements[3][0] },
            {elements[0][1], elements[1][1], elements[2][1], elements[3][1] },
            {elements[0][2], elements[1][2], elements[2][2], elements[3][2] },
            {elements[0][3], elements[1][3], elements[2][3], elements[3][3] }
        };
    }

    float Matrix4::determinant() const {
        return elements[0][0] * (elements[1][1] * (elements[2][2] * elements[3][3] - elements[2][3] * elements[3][2]) + elements[1][2] * (elements[2][3] * elements[3][1] - elements[2][1] * elements[3][3]) + elements[1][3] * (elements[2][1] * elements[3][2] - elements[2][2] * elements[3][1])) +
               elements[0][1] * (elements[1][0] * (elements[2][3] * elements[3][2] - elements[2][2] * elements[3][3]) + elements[1][2] * (elements[2][0] * elements[3][3] - elements[2][3] * elements[3][0]) + elements[1][3] * (elements[2][2] * elements[3][0] - elements[2][0] * elements[3][2])) +
               elements[0][2] * (elements[1][0] * (elements[2][1] * elements[3][3] - elements[2][3] * elements[3][1]) + elements[1][1] * (elements[2][3] * elements[3][0] - elements[2][0] * elements[3][3]) + elements[1][3] * (elements[2][0] * elements[3][1] - elements[2][1] * elements[3][0])) +
               elements[0][3] * (elements[1][0] * (elements[2][2] * elements[3][1] - elements[2][1] * elements[3][2]) + elements[1][1] * (elements[2][0] * elements[3][2] - elements[2][2] * elements[3][0]) + elements[1][2] * (elements[2][1] * elements[3][0] - elements[2][0] * elements[3][1]));
    }

    void Matrix4::setIdentity() {
        elements[0] = {1.0f, 0.0f, 0.0f, 0.0f};
        elements[1] = {0.0f, 1.0f, 0.0f, 0.0f};
        elements[2] = {0.0f, 0.0f, 1.0f, 0.0f};
        elements[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    }

    void Matrix4::setZero() {
        elements[0] = {0.0f, 0.0f, 0.0f, 0.0f};
        elements[1] = {0.0f, 0.0f, 0.0f, 0.0f};
        elements[2] = {0.0f, 0.0f, 0.0f, 0.0f};
        elements[3] = {0.0f, 0.0f, 0.0f, 0.0f};
    }

    void Matrix4::setDiagonal(const Vector4 &dig) {
        elements[0] = {dig.x, 0.0f, 0.0f, 0.0f};
        elements[1] = {0.0f, dig.y, 0.0f, 0.0f};
        elements[2] = {0.0f, 0.0f, dig.z, 0.0f};
        elements[3] = {0.0f, 0.0f, 0.0f, dig.w};
    }

    Vector4 &Matrix4::operator[](size_t row) {
        return elements[row];
    }

    const Vector4 &Matrix4::operator[](size_t row) const {
        return elements[row];
    }

    Matrix4 Matrix4::operator-() const {
        return {-elements[0], -elements[1], -elements[2], -elements[3]};
    }

    void Matrix4::operator+=(const Matrix4 &m) {
        *this = *this + m;
    }

    void Matrix4::operator-=(const Matrix4 &m) {
        *this = *this - m;
    }

    void Matrix4::operator*=(float n) {
        *this = *this * n;
    }

    void Matrix4::operator*=(const Matrix4 &m) {
        *this = *this * m;
    }

    void Matrix4::operator/=(float n) {
        *this = *this / n;
    }

    Matrix4 Matrix4::operator+(const Matrix4 &m) const {
        return {elements[0] + m[0], elements[1] + m[1], elements[2] + m[2], elements[3] + m[3]};
    }

    Matrix4 Matrix4::operator-(const Matrix4 &m) const {
        return {elements[0] - m[0], elements[1] - m[1], elements[2] - m[2], elements[3] - m[3]};
    }

    Matrix4 Matrix4::operator*(float n) const {
        return {elements[0] * n, elements[1] * n, elements[2] * n, elements[3] * n};
    }

    Vector4 Matrix4::operator*(const Vector4 &v) const {
        return {elements[0].dot(v), elements[1].dot(v), elements[2].dot(v), elements[3].dot(v)};
    }

    Matrix4 Matrix4::operator/(float n) const {
        return {elements[0] / n, elements[1] / n, elements[2] / n, elements[3] / n};
    }

    bool Matrix4::operator==(const Matrix4 &m) const {
        return elements[0] == m[0] && elements[1] == m[1] && elements[2] == m[2] && elements[3] == m[3];
    }

    bool Matrix4::operator!=(const Matrix4 &m) const {
        return elements[0] != m[0] && elements[1] != m[1] && elements[2] != m[2] && elements[3] != m[3];
    }

    using mat4 = Matrix4;


} // namespace re


#endif //RAVENENGINE_MATRIX4_HPP
