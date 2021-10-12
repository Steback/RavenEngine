#include "Matrix3.hpp"

#include <cstring>


#define NEW_LINE(newLine) (newLine ? '\n' : ' ')


namespace re {

    Matrix3::Matrix3() = default;

    Matrix3::Matrix3(float n) {
        values[0][0] = n;
        values[1][1] = n;
        values[2][2] = n;
    }

    Matrix3::Matrix3(const Vector3 &a, const Vector3 &b, const Vector3& c) {
        values[0] = a;
        values[1] = b;
        values[2] = c;
    }

    Matrix3::Matrix3(const Vector3 *m) {
        values[0] = m[0];
        values[1] = m[1];
        values[2] = m[2];
    }

    Matrix3::Matrix3(const float *p) {
        values[0] = {p[0], p[1], p[2]};
        values[1] = {p[3], p[4], p[5]};
        values[2] = {p[6], p[7], p[8]};
    }

    Matrix3::Matrix3(const double *p) {
        values[0] = {static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2])};
        values[1] = {static_cast<float>(p[3]), static_cast<float>(p[4]), static_cast<float>(p[5])};
        values[2] = {static_cast<float>(p[6]), static_cast<float>(p[7]), static_cast<float>(p[8])};
    }

    Matrix3::Matrix3(const Matrix3 &m) = default;

    Matrix3 &Matrix3::operator=(const Matrix3 &m) {
        std::memcpy(&values, m.values, 9 * sizeof(float));
        return *this;
    }

    Vector3 &Matrix3::operator[](size_t row) {
        return values[row];
    }

    const Vector3 &Matrix3::operator[](size_t row) const {
        return values[row];
    }

    bool Matrix3::operator==(const Matrix3 &m) const {
        return values[0] == m[0] && values[1] == m[1] && values[2] == m[2];
    }

    bool Matrix3::operator!=(const Matrix3 &m) const {
        return values[0] != m[0] && values[1] != m[1] && values[2] != m[2];
    }

    Matrix3 Matrix3::operator-() const {
        return {-values[0], -values[1], -values[2]};
    }

    Matrix3 Matrix3::operator+(const Matrix3 &m) const {
        return {values[0] + m[0], values[1] + m[1], values[2] + m[2]};
    }

    Matrix3 Matrix3::operator-(const Matrix3 &m) const {
        return {values[0] - m[0], values[1] - m[1], values[2] - m[2]};
    }

    Matrix3 Matrix3::operator*(float n) const {
        return {values[0] * n, values[1] * n, values[2] * n};
    }

    Vector3 Matrix3::operator*(const Vector3 &v) const {
        return {values[0].dot(v), values[1].dot(v), values[2].dot(v)};
    }

    Matrix3 Matrix3::operator*(const Matrix3 &m) const {
        float const SrcA00 = values[0][0];
        float const SrcA01 = values[0][1];
        float const SrcA02 = values[0][2];
        float const SrcA10 = values[1][0];
        float const SrcA11 = values[1][1];
        float const SrcA12 = values[1][2];
        float const SrcA20 = values[2][0];
        float const SrcA21 = values[2][1];
        float const SrcA22 = values[2][2];

        float const SrcB00 = m[0][0];
        float const SrcB01 = m[0][1];
        float const SrcB02 = m[0][2];
        float const SrcB10 = m[1][0];
        float const SrcB11 = m[1][1];
        float const SrcB12 = m[1][2];
        float const SrcB20 = m[2][0];
        float const SrcB21 = m[2][1];
        float const SrcB22 = m[2][2];

        return {
            {
                SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02,
                SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02,
                SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02
            }, {
                SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12,
                SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12,
                SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12
            }, {
                SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22,
                SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22,
                SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22
            }
        };
    }

    Matrix3 Matrix3::operator/(float n) const {
        return {values[0] / n, values[1] / n, values[2] / n};
    }

    Matrix3 &Matrix3::operator+=(const Matrix3 &m) {
        return *this = *this + m;
    }

    Matrix3 &Matrix3::operator-=(const Matrix3 &m) {
        return *this = *this - m;
    }

    Matrix3 &Matrix3::operator*=(float n) {
        return *this = *this * n;
    }

    Matrix3 &Matrix3::operator*=(const Matrix3 &m) {
        return *this = *this * m;
    }

    Matrix3 &Matrix3::operator/=(float n) {
        return *this = *this / n;
    }

    float Matrix3::determinant() const {
        return (values[0][0] * values[1][1] * values[2][2]) +
               (values[0][1] * values[1][2] * values[2][0]) +
               (values[0][2] * values[1][0] * values[2][1]) -
               (values[0][2] * values[1][1] * values[2][0]) -
               (values[0][0] * values[1][2] * values[2][1]) -
               (values[0][1] * values[1][0] * values[2][2]);
    }

    Matrix3 Matrix3::adjugate() const {
        return {
            {
                values[1][1] * values[2][2] - values[1][2] * values[2][1],
                values[0][2] * values[2][1] - values[0][1] * values[2][2],
                values[0][1] * values[1][2] - values[0][2] * values[1][1]
            },{
                values[1][2] * values[2][0] - values[1][0] * values[2][2],
                values[0][0] * values[2][2] - values[0][2] * values[2][0],
                values[0][2] * values[1][0] - values[0][0] * values[1][2]
            },{
                values[1][0] * values[2][1] - values[1][1] * values[2][0],
                values[0][1] * values[2][0] - values[0][0] * values[2][1],
                values[0][0] * values[1][1] - values[0][1] * values[1][0]
            }
        };
    }

    Matrix3 Matrix3::inverse() const {
        return adjugate() / determinant();
    }

    Matrix3 Matrix3::transpose() const {
        return {{values[0][0], values[1][0], values[2][0]},
                {values[0][1], values[1][1], values[2][1]},
                {values[0][2], values[1][2], values[2][2]}};
    }

    std::string Matrix3::str(bool newLine) const {
        return values[0].str() + NEW_LINE(newLine) +
               values[1].str() + NEW_LINE(newLine) +
               values[2].str() + NEW_LINE(newLine);
    }

} // namespace re