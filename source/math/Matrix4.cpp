#include "Matrix4.hpp"


#define NEW_LINE(newLine) (newLine ? '\n' : ' ')


namespace re {

    Matrix4::Matrix4() = default;

    Matrix4::Matrix4(float n) {
        rows[0][0] = n;
        rows[1][1] = n;
        rows[2][2] = n;
        rows[3][3] = n;
    }

    Matrix4::Matrix4(const Vector4 &a, const Vector4 &b, const Vector4 &c, const Vector4 &d) {
        rows[0] = a;
        rows[1] = b;
        rows[2] = c;
        rows[3] = d;
    }

    Matrix4::Matrix4(const Vector4 *m) {
        rows[0] = m[0];
        rows[1] = m[1];
        rows[2] = m[2];
        rows[3] = m[3];
    }

    Matrix4::Matrix4(const float *p) {
        rows[0] = {p[0], p[1], p[2], p[3]};
        rows[1] = {p[4], p[5], p[6], p[7]};
        rows[2] = {p[8], p[9], p[10], p[11]};
        rows[3] = {p[12], p[13], p[14], p[15]};
    }

    Matrix4::Matrix4(const Matrix4 &m) = default;

    Matrix4 &Matrix4::operator=(const Matrix4 &m) = default;

    Vector4 &Matrix4::operator[](size_t row) {
        return rows[row];
    }

    const Vector4 &Matrix4::operator[](size_t row) const {
        return rows[row];
    }

    bool Matrix4::operator==(const Matrix4 &m) const {
        return rows[0] == m[0] && rows[1] == m[1] && rows[2] == m[2] && rows[3] == m[3];
    }

    bool Matrix4::operator!=(const Matrix4 &m) const {
        return rows[0] != m[0] && rows[1] != m[1] && rows[2] != m[2] && rows[3] != m[3];
    }

    Matrix4 Matrix4::operator-() const {
        return {-rows[0], -rows[1], -rows[2], -rows[3]};
    }

    Matrix4 Matrix4::operator+(const Matrix4 &m) const {
        return {rows[0] + m[0], rows[1] + m[1], rows[2] + m[2], rows[3] + m[3]};
    }

    Matrix4 Matrix4::operator-(const Matrix4 &m) const {
        return {rows[0] - m[0], rows[1] - m[1], rows[2] - m[2], rows[3] - m[3]};
    }

    Matrix4 Matrix4::operator*(float n) const {
        return {rows[0] * n, rows[1] * n, rows[2] * n, rows[3] * n};
    }

    Vector4 Matrix4::operator*(const Vector4 &v) const {
        return {rows[0] * v, rows[1] * v, rows[2] * v, rows[3] * v};
    }

    Matrix4 Matrix4::operator*(const Matrix4 &m) const {
        // TODO: I don't understand why GML use this method and how it works...
        const vec4 srcA0 = rows[0];
        const vec4 srcA1 = rows[1];
        const vec4 srcA2 = rows[2];
        const vec4 srcA3 = rows[3];

        const vec4 srcB0 = m[0];
        const vec4 srcB1 = m[1];
        const vec4 srcB2 = m[2];
        const vec4 srcB3 = m[3];

        mat4 Result;
        Result[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
        Result[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
        Result[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
        Result[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];

        return Result;
//        return {
//            {
//                rows[0][0] * m[0][0] + rows[0][1] * m[1][0] + rows[0][2] * m[2][0] + rows[0][3] * m[3][0],
//                rows[0][0] * m[0][1] + rows[0][1] * m[1][1] + rows[0][2] * m[2][1] + rows[0][3] * m[3][1],
//                rows[0][0] * m[0][2] + rows[0][1] * m[1][2] + rows[0][2] * m[2][2] + rows[0][3] * m[3][2],
//                rows[0][0] * m[0][3] + rows[0][1] * m[1][3] + rows[0][2] * m[2][3] + rows[0][3] * m[3][3]
//            }, {
//                rows[1][0] * m[0][0] + rows[1][1] * m[1][0] + rows[1][2] * m[2][0] + rows[1][3] * m[3][0],
//                rows[1][0] * m[0][1] + rows[1][1] * m[1][1] + rows[1][2] * m[2][1] + rows[1][3] * m[3][1],
//                rows[1][0] * m[0][2] + rows[1][1] * m[1][2] + rows[1][2] * m[2][2] + rows[1][3] * m[3][2],
//                rows[1][0] * m[0][3] + rows[1][1] * m[1][3] + rows[1][2] * m[2][3] + rows[1][3] * m[3][3]
//            }, {
//                rows[2][0] * m[0][0] + rows[2][1] * m[1][0] + rows[2][2] * m[2][0] + rows[2][3] * m[3][0],
//                rows[2][0] * m[0][1] + rows[2][1] * m[1][1] + rows[2][2] * m[2][1] + rows[2][3] * m[3][1],
//                rows[2][0] * m[0][2] + rows[2][1] * m[1][2] + rows[2][2] * m[2][2] + rows[2][3] * m[3][2],
//                rows[2][0] * m[0][3] + rows[2][1] * m[1][3] + rows[2][2] * m[2][3] + rows[2][3] * m[3][3]
//            }, {
//                rows[3][0] * m[0][0] + rows[3][1] * m[1][0] + rows[3][2] * m[2][0] + rows[3][3] * m[3][0],
//                rows[3][0] * m[0][1] + rows[3][1] * m[1][1] + rows[3][2] * m[2][1] + rows[3][3] * m[3][1],
//                rows[3][0] * m[0][2] + rows[3][1] * m[1][2] + rows[3][2] * m[2][2] + rows[3][3] * m[3][2],
//                rows[3][0] * m[0][3] + rows[3][1] * m[1][3] + rows[3][2] * m[2][3] + rows[3][3] * m[3][3]
//            }
//        };
    }

    Matrix4 Matrix4::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
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
        return rows[0][0] * (rows[1][1] * (rows[2][2] * rows[3][3] - rows[2][3] * rows[3][2]) + rows[1][2] * (rows[2][3] * rows[3][1] - rows[2][1] * rows[3][3]) + rows[1][3] * (rows[2][1] * rows[3][2] - rows[2][2] * rows[3][1])) +
               rows[0][1] * (rows[1][0] * (rows[2][3] * rows[3][2] - rows[2][2] * rows[3][3]) + rows[1][2] * (rows[2][0] * rows[3][3] - rows[2][3] * rows[3][0]) + rows[1][3] * (rows[2][2] * rows[3][0] - rows[2][0] * rows[3][2])) +
               rows[0][2] * (rows[1][0] * (rows[2][1] * rows[3][3] - rows[2][3] * rows[3][1]) + rows[1][1] * (rows[2][3] * rows[3][0] - rows[2][0] * rows[3][3]) + rows[1][3] * (rows[2][0] * rows[3][1] - rows[2][1] * rows[3][0])) +
               rows[0][3] * (rows[1][0] * (rows[2][2] * rows[3][1] - rows[2][1] * rows[3][2]) + rows[1][1] * (rows[2][0] * rows[3][2] - rows[2][2] * rows[3][0]) + rows[1][2] * (rows[2][1] * rows[3][0] - rows[2][0] * rows[3][1]));
    }

    Matrix4 Matrix4::adjugate() const {
        return {
            {
                rows[1][1] * (rows[2][2] * rows[3][3] - rows[2][3] * rows[3][2]) +
                    rows[1][2] * (rows[2][3] * rows[3][1] - rows[2][1] * rows[3][3]) +
                    rows[1][3] * (rows[2][1] * rows[3][2] - rows[2][2] * rows[3][1]),
                rows[0][1] * (rows[2][3] * rows[3][2] - rows[2][2] * rows[3][3]) +
                    rows[0][2] * (rows[2][1] * rows[3][3] - rows[2][3] * rows[3][1]) +
                    rows[0][3] * (rows[2][2] * rows[3][1] - rows[2][1] * rows[3][2]),
                rows[0][1] * (rows[1][2] * rows[3][3] - rows[1][3] * rows[3][2]) +
                    rows[0][2] * (rows[1][3] * rows[3][1] - rows[1][1] * rows[3][3]) +
                    rows[0][3] * (rows[1][1] * rows[3][2] - rows[1][2] * rows[3][1]),
                rows[0][1] * (rows[1][3] * rows[2][2] - rows[1][2] * rows[2][3]) +
                    rows[0][2] * (rows[1][1] * rows[2][3] - rows[1][3] * rows[2][1]) +
                    rows[0][3] * (rows[1][2] * rows[2][1] - rows[1][1] * rows[2][2])
            }, {
                rows[1][0] * (rows[2][3] * rows[3][2] - rows[2][2] * rows[3][3]) +
                    rows[1][2] * (rows[2][0] * rows[3][3] - rows[2][3] * rows[3][0]) +
                    rows[1][3] * (rows[2][2] * rows[3][0] - rows[2][0] * rows[3][2]),
                rows[0][0] * (rows[2][2] * rows[3][3] - rows[2][3] * rows[3][2]) +
                    rows[0][2] * (rows[2][3] * rows[3][0] - rows[2][0] * rows[3][3]) +
                    rows[0][3] * (rows[2][0] * rows[3][2] - rows[2][2] * rows[3][0]),
                rows[0][0] * (rows[1][3] * rows[3][2] - rows[1][2] * rows[3][3]) +
                    rows[0][2] * (rows[1][0] * rows[3][3] - rows[1][3] * rows[3][0]) +
                    rows[0][3] * (rows[1][2] * rows[3][0] - rows[1][0] * rows[3][2]),
                rows[0][0] * (rows[1][2] * rows[2][3] - rows[1][3] * rows[2][2]) +
                    rows[0][2] * (rows[1][3] * rows[2][0] - rows[1][0] * rows[2][3]) +
                    rows[0][3] * (rows[1][0] * rows[2][2] - rows[1][2] * rows[2][0]),
            },{
                rows[1][0] * (rows[2][1] * rows[3][3] - rows[2][3] * rows[3][1]) +
                    rows[1][1] * (rows[2][3] * rows[3][0] - rows[2][0] * rows[3][3]) +
                    rows[1][3] * (rows[2][0] * rows[3][1] - rows[2][1] * rows[3][0]),
                rows[0][0] * (rows[2][3] * rows[3][1] - rows[2][1] * rows[3][3]) +
                    rows[0][1] * (rows[2][0] * rows[3][3] - rows[2][3] * rows[3][0]) +
                    rows[0][3] * (rows[2][1] * rows[3][0] - rows[2][0] * rows[3][1]),
                rows[0][0] * (rows[1][1] * rows[3][3] - rows[1][3] * rows[3][1]) +
                    rows[0][1] * (rows[1][3] * rows[3][0] - rows[1][0] * rows[3][3]) +
                    rows[0][3] * (rows[1][0] * rows[3][1] - rows[1][1] * rows[3][0]),
                rows[0][0] * (rows[1][3] * rows[2][1] - rows[1][1] * rows[2][3]) +
                    rows[0][1] * (rows[1][0] * rows[2][3] - rows[1][3] * rows[2][0]) +
                    rows[0][3] * (rows[1][1] * rows[2][0] - rows[1][0] * rows[2][1]),
            }, {
                rows[1][0] * (rows[2][2] * rows[3][1] - rows[2][1] * rows[3][2]) +
                    rows[1][1] * (rows[2][0] * rows[3][2] - rows[2][2] * rows[3][0]) +
                    rows[1][2] * (rows[2][1] * rows[3][0] - rows[2][0] * rows[3][1]),
                rows[0][0] * (rows[2][1] * rows[3][2] - rows[2][2] * rows[3][1]) +
                    rows[0][1] * (rows[2][2] * rows[3][0] - rows[2][0] * rows[3][2]) +
                    rows[0][2] * (rows[2][0] * rows[3][1] - rows[2][1] * rows[3][0]),
                rows[0][0] * (rows[1][2] * rows[3][1] - rows[1][1] * rows[3][2]) +
                    rows[0][1] * (rows[1][0] * rows[3][2] - rows[1][2] * rows[3][0]) +
                    rows[0][2] * (rows[1][1] * rows[3][0] - rows[1][0] * rows[3][1]),
                rows[0][0] * (rows[1][1] * rows[2][2] - rows[1][2] * rows[2][1]) +
                    rows[0][1] * (rows[1][2] * rows[2][0] - rows[1][0] * rows[2][2]) +
                    rows[0][2] * (rows[1][0] * rows[2][1] - rows[1][1] * rows[2][0])
            }
        };
    }

    Matrix4 Matrix4::inverse() const {
        return adjugate() / determinant();
    }

    Matrix4 Matrix4::transpose() const {
        return {
            { rows[0][0], rows[1][0], rows[2][0], rows[3][0] },
            { rows[0][1], rows[1][1], rows[2][1], rows[3][1] },
            { rows[0][2], rows[1][2], rows[2][2], rows[3][2] },
            { rows[0][3], rows[1][3], rows[2][3], rows[3][3] }
        };
    }

    std::string Matrix4::str(bool newLine) const {
        return rows[0].str() + NEW_LINE(newLine) +
               rows[1].str() + NEW_LINE(newLine) +
               rows[2].str() + NEW_LINE(newLine) +
               rows[3].str() + NEW_LINE(newLine);
    }

} // namespace re