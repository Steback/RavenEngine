#include "Matrix4.hpp"

#include <cstring>


#define NEW_LINE(newLine) (newLine ? '\n' : ' ')


namespace re {

    Matrix4::Matrix4() = default;

    Matrix4::Matrix4(float n) {
        values[0][0] = n;
        values[1][1] = n;
        values[2][2] = n;
        values[3][3] = n;
    }

    Matrix4::Matrix4(const Vector4 &a, const Vector4 &b, const Vector4 &c, const Vector4 &d) {
        values[0] = a;
        values[1] = b;
        values[2] = c;
        values[3] = d;
    }

    Matrix4::Matrix4(const Vector4 *m) {
        values[0] = m[0];
        values[1] = m[1];
        values[2] = m[2];
        values[3] = m[3];
    }

    Matrix4::Matrix4(const float *p) {
        values[0] = {p[0], p[1], p[2], p[3]};
        values[1] = {p[4], p[5], p[6], p[7]};
        values[2] = {p[8], p[9], p[10], p[11]};
        values[3] = {p[12], p[13], p[14], p[15]};
    }

    Matrix4::Matrix4(const double *p) {
        values[0] = {static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2]),static_cast<float>(p[3])};
        values[1] = {static_cast<float>(p[4]), static_cast<float>(p[5]), static_cast<float>(p[6]),static_cast<float>(p[7])};
        values[2] = {static_cast<float>(p[8]), static_cast<float>(p[9]), static_cast<float>(p[10]),static_cast<float>(p[11])};
        values[2] = {static_cast<float>(p[12]), static_cast<float>(p[13]), static_cast<float>(p[14]),static_cast<float>(p[15])};
    }

    Matrix4::Matrix4(const Matrix4 &m) = default;

    Matrix4 &Matrix4::operator=(const Matrix4 &m) {
        std::memcpy(&values, m.values, 16 * sizeof(float));
        return *this;
    }

    Vector4 &Matrix4::operator[](size_t row) {
        return values[row];
    }

    const Vector4 &Matrix4::operator[](size_t row) const {
        return values[row];
    }

    bool Matrix4::operator==(const Matrix4 &m) const {
        return values[0] == m[0] && values[1] == m[1] && values[2] == m[2] && values[3] == m[3];
    }

    bool Matrix4::operator!=(const Matrix4 &m) const {
        return values[0] != m[0] && values[1] != m[1] && values[2] != m[2] && values[3] != m[3];
    }

    Matrix4 Matrix4::operator-() const {
        return {-values[0], -values[1], -values[2], -values[3]};
    }

    Matrix4 Matrix4::operator+(const Matrix4 &m) const {
        return {values[0] + m[0], values[1] + m[1], values[2] + m[2], values[3] + m[3]};
    }

    Matrix4 Matrix4::operator-(const Matrix4 &m) const {
        return {values[0] - m[0], values[1] - m[1], values[2] - m[2], values[3] - m[3]};
    }

    Matrix4 Matrix4::operator*(float n) const {
        return {values[0] * n, values[1] * n, values[2] * n, values[3] * n};
    }

    Vector4 Matrix4::operator*(const Vector4 &v) const {
        return {values[0].dot(v), values[1].dot(v), values[2].dot(v), values[3].dot(v)};
    }

    Matrix4 Matrix4::operator*(const Matrix4 &m) const {
        const vec4 srcA0 = values[0];
        const vec4 srcA1 = values[1];
        const vec4 srcA2 = values[2];
        const vec4 srcA3 = values[3];

        const vec4 srcB0 = m[0];
        const vec4 srcB1 = m[1];
        const vec4 srcB2 = m[2];
        const vec4 srcB3 = m[3];

        return {
            {srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3]},
            {srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3]},
            {srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3]},
            {srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3]}
        };
    }

    Matrix4 Matrix4::operator/(float n) const {
        return {values[0] / n, values[1] / n, values[2] / n, values[3] / n};
    }

    Matrix4 &Matrix4::operator+=(const Matrix4 &m) {
        return *this = *this + m;
    }

    Matrix4 &Matrix4::operator-=(const Matrix4 &m) {
        return *this = *this - m;
    }

    Matrix4 &Matrix4::operator*=(float n) {
        return *this = *this * n;
    }

    Matrix4 &Matrix4::operator*=(const Matrix4 &m) {
        return *this = *this * m;
    }

    Matrix4 &Matrix4::operator/=(float n) {
        return *this = *this / n;
    }

    float Matrix4::determinant() const {
        return values[0][0] * (values[1][1] * (values[2][2] * values[3][3] - values[2][3] * values[3][2]) + values[1][2] * (values[2][3] * values[3][1] - values[2][1] * values[3][3]) + values[1][3] * (values[2][1] * values[3][2] - values[2][2] * values[3][1])) +
               values[0][1] * (values[1][0] * (values[2][3] * values[3][2] - values[2][2] * values[3][3]) + values[1][2] * (values[2][0] * values[3][3] - values[2][3] * values[3][0]) + values[1][3] * (values[2][2] * values[3][0] - values[2][0] * values[3][2])) +
               values[0][2] * (values[1][0] * (values[2][1] * values[3][3] - values[2][3] * values[3][1]) + values[1][1] * (values[2][3] * values[3][0] - values[2][0] * values[3][3]) + values[1][3] * (values[2][0] * values[3][1] - values[2][1] * values[3][0])) +
               values[0][3] * (values[1][0] * (values[2][2] * values[3][1] - values[2][1] * values[3][2]) + values[1][1] * (values[2][0] * values[3][2] - values[2][2] * values[3][0]) + values[1][2] * (values[2][1] * values[3][0] - values[2][0] * values[3][1]));
    }

    Matrix4 Matrix4::adjugate() const {
        return {
            {
                values[1][1] * (values[2][2] * values[3][3] - values[2][3] * values[3][2]) +
                values[1][2] * (values[2][3] * values[3][1] - values[2][1] * values[3][3]) +
                values[1][3] * (values[2][1] * values[3][2] - values[2][2] * values[3][1]),
                values[0][1] * (values[2][3] * values[3][2] - values[2][2] * values[3][3]) +
                values[0][2] * (values[2][1] * values[3][3] - values[2][3] * values[3][1]) +
                values[0][3] * (values[2][2] * values[3][1] - values[2][1] * values[3][2]),
                values[0][1] * (values[1][2] * values[3][3] - values[1][3] * values[3][2]) +
                values[0][2] * (values[1][3] * values[3][1] - values[1][1] * values[3][3]) +
                values[0][3] * (values[1][1] * values[3][2] - values[1][2] * values[3][1]),
                values[0][1] * (values[1][3] * values[2][2] - values[1][2] * values[2][3]) +
                values[0][2] * (values[1][1] * values[2][3] - values[1][3] * values[2][1]) +
                values[0][3] * (values[1][2] * values[2][1] - values[1][1] * values[2][2])
            }, {
                values[1][0] * (values[2][3] * values[3][2] - values[2][2] * values[3][3]) +
                values[1][2] * (values[2][0] * values[3][3] - values[2][3] * values[3][0]) +
                values[1][3] * (values[2][2] * values[3][0] - values[2][0] * values[3][2]),
                values[0][0] * (values[2][2] * values[3][3] - values[2][3] * values[3][2]) +
                values[0][2] * (values[2][3] * values[3][0] - values[2][0] * values[3][3]) +
                values[0][3] * (values[2][0] * values[3][2] - values[2][2] * values[3][0]),
                values[0][0] * (values[1][3] * values[3][2] - values[1][2] * values[3][3]) +
                values[0][2] * (values[1][0] * values[3][3] - values[1][3] * values[3][0]) +
                values[0][3] * (values[1][2] * values[3][0] - values[1][0] * values[3][2]),
                values[0][0] * (values[1][2] * values[2][3] - values[1][3] * values[2][2]) +
                values[0][2] * (values[1][3] * values[2][0] - values[1][0] * values[2][3]) +
                values[0][3] * (values[1][0] * values[2][2] - values[1][2] * values[2][0]),
            },{
                values[1][0] * (values[2][1] * values[3][3] - values[2][3] * values[3][1]) +
                values[1][1] * (values[2][3] * values[3][0] - values[2][0] * values[3][3]) +
                values[1][3] * (values[2][0] * values[3][1] - values[2][1] * values[3][0]),
                values[0][0] * (values[2][3] * values[3][1] - values[2][1] * values[3][3]) +
                values[0][1] * (values[2][0] * values[3][3] - values[2][3] * values[3][0]) +
                values[0][3] * (values[2][1] * values[3][0] - values[2][0] * values[3][1]),
                values[0][0] * (values[1][1] * values[3][3] - values[1][3] * values[3][1]) +
                values[0][1] * (values[1][3] * values[3][0] - values[1][0] * values[3][3]) +
                values[0][3] * (values[1][0] * values[3][1] - values[1][1] * values[3][0]),
                values[0][0] * (values[1][3] * values[2][1] - values[1][1] * values[2][3]) +
                values[0][1] * (values[1][0] * values[2][3] - values[1][3] * values[2][0]) +
                values[0][3] * (values[1][1] * values[2][0] - values[1][0] * values[2][1]),
            }, {
                values[1][0] * (values[2][2] * values[3][1] - values[2][1] * values[3][2]) +
                values[1][1] * (values[2][0] * values[3][2] - values[2][2] * values[3][0]) +
                values[1][2] * (values[2][1] * values[3][0] - values[2][0] * values[3][1]),
                values[0][0] * (values[2][1] * values[3][2] - values[2][2] * values[3][1]) +
                values[0][1] * (values[2][2] * values[3][0] - values[2][0] * values[3][2]) +
                values[0][2] * (values[2][0] * values[3][1] - values[2][1] * values[3][0]),
                values[0][0] * (values[1][2] * values[3][1] - values[1][1] * values[3][2]) +
                values[0][1] * (values[1][0] * values[3][2] - values[1][2] * values[3][0]) +
                values[0][2] * (values[1][1] * values[3][0] - values[1][0] * values[3][1]),
                values[0][0] * (values[1][1] * values[2][2] - values[1][2] * values[2][1]) +
                values[0][1] * (values[1][2] * values[2][0] - values[1][0] * values[2][2]) +
                values[0][2] * (values[1][0] * values[2][1] - values[1][1] * values[2][0])
            }
        };
    }

    Matrix4 Matrix4::inverse() const {
        return adjugate() / determinant();
    }

    Matrix4 Matrix4::transpose() const {
        return {
            {values[0][0], values[1][0], values[2][0], values[3][0] },
            {values[0][1], values[1][1], values[2][1], values[3][1] },
            {values[0][2], values[1][2], values[2][2], values[3][2] },
            {values[0][3], values[1][3], values[2][3], values[3][3] }
        };
    }

    std::string Matrix4::str(bool newLine) const {
        return values[0].str() + NEW_LINE(newLine) +
               values[1].str() + NEW_LINE(newLine) +
               values[2].str() + NEW_LINE(newLine) +
               values[3].str() + NEW_LINE(newLine);
    }

} // namespace re