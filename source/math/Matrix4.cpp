#include "Matrix4.hpp"


namespace re {

    void Matrix4::invert() {
        *this = inverted();
    }

    Matrix4 Matrix4::inverted() const {
        return adjugate() / determinant();
    }

    Matrix4 Matrix4::adjugate() const {
        return {
            {
                elements[1][1] * (elements[2][2] * elements[3][3] - elements[2][3] * elements[3][2]) +
                elements[1][2] * (elements[2][3] * elements[3][1] - elements[2][1] * elements[3][3]) +
                elements[1][3] * (elements[2][1] * elements[3][2] - elements[2][2] * elements[3][1]),
                elements[0][1] * (elements[2][3] * elements[3][2] - elements[2][2] * elements[3][3]) +
                elements[0][2] * (elements[2][1] * elements[3][3] - elements[2][3] * elements[3][1]) +
                elements[0][3] * (elements[2][2] * elements[3][1] - elements[2][1] * elements[3][2]),
                elements[0][1] * (elements[1][2] * elements[3][3] - elements[1][3] * elements[3][2]) +
                elements[0][2] * (elements[1][3] * elements[3][1] - elements[1][1] * elements[3][3]) +
                elements[0][3] * (elements[1][1] * elements[3][2] - elements[1][2] * elements[3][1]),
                elements[0][1] * (elements[1][3] * elements[2][2] - elements[1][2] * elements[2][3]) +
                elements[0][2] * (elements[1][1] * elements[2][3] - elements[1][3] * elements[2][1]) +
                elements[0][3] * (elements[1][2] * elements[2][1] - elements[1][1] * elements[2][2])
            }, {
                elements[1][0] * (elements[2][3] * elements[3][2] - elements[2][2] * elements[3][3]) +
                elements[1][2] * (elements[2][0] * elements[3][3] - elements[2][3] * elements[3][0]) +
                elements[1][3] * (elements[2][2] * elements[3][0] - elements[2][0] * elements[3][2]),
                elements[0][0] * (elements[2][2] * elements[3][3] - elements[2][3] * elements[3][2]) +
                elements[0][2] * (elements[2][3] * elements[3][0] - elements[2][0] * elements[3][3]) +
                elements[0][3] * (elements[2][0] * elements[3][2] - elements[2][2] * elements[3][0]),
                elements[0][0] * (elements[1][3] * elements[3][2] - elements[1][2] * elements[3][3]) +
                elements[0][2] * (elements[1][0] * elements[3][3] - elements[1][3] * elements[3][0]) +
                elements[0][3] * (elements[1][2] * elements[3][0] - elements[1][0] * elements[3][2]),
                elements[0][0] * (elements[1][2] * elements[2][3] - elements[1][3] * elements[2][2]) +
                elements[0][2] * (elements[1][3] * elements[2][0] - elements[1][0] * elements[2][3]) +
                elements[0][3] * (elements[1][0] * elements[2][2] - elements[1][2] * elements[2][0]),
            },{
                elements[1][0] * (elements[2][1] * elements[3][3] - elements[2][3] * elements[3][1]) +
                elements[1][1] * (elements[2][3] * elements[3][0] - elements[2][0] * elements[3][3]) +
                elements[1][3] * (elements[2][0] * elements[3][1] - elements[2][1] * elements[3][0]),
                elements[0][0] * (elements[2][3] * elements[3][1] - elements[2][1] * elements[3][3]) +
                elements[0][1] * (elements[2][0] * elements[3][3] - elements[2][3] * elements[3][0]) +
                elements[0][3] * (elements[2][1] * elements[3][0] - elements[2][0] * elements[3][1]),
                elements[0][0] * (elements[1][1] * elements[3][3] - elements[1][3] * elements[3][1]) +
                elements[0][1] * (elements[1][3] * elements[3][0] - elements[1][0] * elements[3][3]) +
                elements[0][3] * (elements[1][0] * elements[3][1] - elements[1][1] * elements[3][0]),
                elements[0][0] * (elements[1][3] * elements[2][1] - elements[1][1] * elements[2][3]) +
                elements[0][1] * (elements[1][0] * elements[2][3] - elements[1][3] * elements[2][0]) +
                elements[0][3] * (elements[1][1] * elements[2][0] - elements[1][0] * elements[2][1]),
            }, {
                elements[1][0] * (elements[2][2] * elements[3][1] - elements[2][1] * elements[3][2]) +
                elements[1][1] * (elements[2][0] * elements[3][2] - elements[2][2] * elements[3][0]) +
                elements[1][2] * (elements[2][1] * elements[3][0] - elements[2][0] * elements[3][1]),
                elements[0][0] * (elements[2][1] * elements[3][2] - elements[2][2] * elements[3][1]) +
                elements[0][1] * (elements[2][2] * elements[3][0] - elements[2][0] * elements[3][2]) +
                elements[0][2] * (elements[2][0] * elements[3][1] - elements[2][1] * elements[3][0]),
                elements[0][0] * (elements[1][2] * elements[3][1] - elements[1][1] * elements[3][2]) +
                elements[0][1] * (elements[1][0] * elements[3][2] - elements[1][2] * elements[3][0]) +
                elements[0][2] * (elements[1][1] * elements[3][0] - elements[1][0] * elements[3][1]),
                elements[0][0] * (elements[1][1] * elements[2][2] - elements[1][2] * elements[2][1]) +
                elements[0][1] * (elements[1][2] * elements[2][0] - elements[1][0] * elements[2][2]) +
                elements[0][2] * (elements[1][0] * elements[2][1] - elements[1][1] * elements[2][0])
            }
        };
    }

    std::string Matrix4::str() const {
        return "[" + std::to_string(elements[0][0]) + " " + std::to_string(elements[0][1]) + " " + std::to_string(elements[0][2]) + " " + std::to_string(elements[0][3]) + "]"
            + "[" + std::to_string(elements[1][0]) + " " + std::to_string(elements[1][1]) + " " + std::to_string(elements[1][2]) + " " + std::to_string(elements[1][3]) + "]"
            + "[" + std::to_string(elements[2][0]) + " " + std::to_string(elements[2][1]) + " " + std::to_string(elements[2][2]) + " " + std::to_string(elements[2][3]) + "]"
            + "[" + std::to_string(elements[3][0]) + " " + std::to_string(elements[3][1]) + " " + std::to_string(elements[3][2]) + " " + std::to_string(elements[3][3]) + "]";
    }

    Matrix4 Matrix4::operator*(const Matrix4 &m) const {
        const vec4 srcA0 = elements[0];
        const vec4 srcA1 = elements[1];
        const vec4 srcA2 = elements[2];
        const vec4 srcA3 = elements[3];

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

} // namespace re