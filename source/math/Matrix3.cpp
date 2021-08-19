#include "Matrix3.hpp"


#define NEW_LINE(newLine) (newLine ? '\n' : ' ')


namespace re {

    Matrix3::Matrix3() = default;

    Matrix3::Matrix3(float n) {
        rows[0][0] = n;
        rows[1][1] = n;
        rows[2][2] = n;
    }

    Matrix3::Matrix3(const Vector3 &a, const Vector3 &b, const Vector3& c) {
        rows[0] = a;
        rows[1] = b;
        rows[2] = c;
    }

    Matrix3::Matrix3(const Vector3 *m) {
        rows[0] = m[0];
        rows[1] = m[1];
        rows[2] = m[2];
    }

    Matrix3::Matrix3(const Matrix3 &m) = default;

    Matrix3 &Matrix3::operator=(const Matrix3 &m) = default;

    Vector3 &Matrix3::operator[](size_t row) {
        return rows[row];
    }

    const Vector3 &Matrix3::operator[](size_t row) const {
        return rows[row];
    }

    Matrix3 Matrix3::operator-() const {
        return {-rows[0], -rows[1], -rows[2]};
    }

    Matrix3 Matrix3::operator+(const Matrix3 &m) const {
        return {rows[0] + m[0], rows[1] + m[1], rows[2] + m[2]};
    }

    Matrix3 Matrix3::operator-(const Matrix3 &m) const {
        return {rows[0] - m[0], rows[1] - m[1], rows[2] - m[2]};
    }

    Matrix3 Matrix3::operator*(float n) const {
        return {rows[0] * n, rows[1] * n, rows[2] * n};
    }

    Vector3 Matrix3::operator*(const Vector3 &v) const {
        return {rows[0] * v, rows[1] * v, rows[2] * v};
    }

    Matrix3 Matrix3::operator*(const Matrix3 &m) const {
        return {
            {
                rows[0][0] * m[0][0] + rows[0][1] * m[1][0] + rows[0][2] * m[2][0],
                rows[0][0] * m[0][1] + rows[0][1] * m[1][1] + rows[0][2] * m[2][1],
                rows[0][0] * m[0][2] + rows[0][1] * m[1][2] + rows[0][2] * m[2][2]
            },{
                rows[1][0] * m[0][0] + rows[1][1] * m[1][0] + rows[1][2] * m[2][0],
                rows[1][0] * m[0][1] + rows[1][1] * m[1][1] + rows[1][2] * m[2][1],
                rows[1][0] * m[0][2] + rows[1][1] * m[1][2] + rows[1][2] * m[2][2]
            },{
                rows[2][0] * m[0][0] + rows[2][1] * m[1][0] + rows[2][2] * m[2][0],
                rows[2][0] * m[0][1] + rows[2][1] * m[1][1] + rows[2][2] * m[2][1],
                rows[2][0] * m[0][2] + rows[2][2] * m[1][0] + rows[2][2] * m[2][2]
            }
        };
    }

    Matrix3 Matrix3::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
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

    bool Matrix3::operator==(const Matrix3 &m) const {
        return rows[0] == m[0] && rows[1] == m[1] && rows[2] == m[2];
    }

    bool Matrix3::operator!=(const Matrix3 &m) const {
        return rows[0] != m[0] && rows[1] != m[1] && rows[2] != m[2];
    }

    float Matrix3::determinant() const {
        return (rows[0][0] * rows[1][1] * rows[2][2]) +
               (rows[0][1] * rows[1][2] * rows[2][0]) +
               (rows[0][2] * rows[1][0] * rows[2][1]) -
               (rows[0][2] * rows[1][1] * rows[2][0]) -
               (rows[0][0] * rows[1][2] * rows[2][1]) -
               (rows[0][1] * rows[1][0] * rows[2][2]);
    }

    Matrix3 Matrix3::adjugate() const {
        return {
            {
                rows[1][1] * rows[2][2] - rows[1][2] * rows[2][1],
                rows[0][2] * rows[2][1] - rows[0][1] * rows[2][2],
                rows[0][1] * rows[1][2] - rows[0][2] * rows[1][1]
                },{
                rows[1][2] * rows[2][0] - rows[1][0] * rows[2][2],
                rows[0][0] * rows[2][2] - rows[0][2] * rows[2][0],
                rows[0][2] * rows[1][0] - rows[0][0] * rows[1][2]
                },{
                rows[1][0] * rows[2][1] - rows[1][1] * rows[2][0],
                rows[0][1] * rows[2][0] - rows[0][0] * rows[2][1],
                rows[0][0] * rows[1][1] - rows[0][1] * rows[1][0]
            }
        };
    }

    Matrix3 Matrix3::inverse() const {
        return adjugate() / determinant();
    }

    Matrix3 Matrix3::transpose() const {
        return {{rows[0][0], rows[1][0], rows[2][0]},
                {rows[0][1], rows[1][1], rows[2][1]},
                {rows[0][2], rows[1][2], rows[2][2]}};
    }

    std::string Matrix3::str(bool newLine) const {
        return  rows[0].str() + NEW_LINE(newLine) +
                rows[1].str() + NEW_LINE(newLine) +
                rows[2].str() + NEW_LINE(newLine);
    }

} // namespace re