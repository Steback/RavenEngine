#include "Basis.hpp"

#include "Math.hpp"


namespace re {

    void Basis::invert() {
        *this = inverted();
    }

    Basis Basis::inverted() {
        float invDet = 1.0f / determinant();
        return {
                {
                        (elements[1][1] * elements[2][2] - elements[1][2] * elements[2][1]) * invDet,
                        (elements[0][2] * elements[2][1] - elements[0][1] * elements[2][2]) * invDet,
                        (elements[0][1] * elements[1][2] - elements[0][2] * elements[1][1]) * invDet
                },{
                        (elements[1][2] * elements[2][0] - elements[1][0] * elements[2][2]) * invDet,
                        (elements[0][0] * elements[2][2] - elements[0][2] * elements[2][0]) * invDet,
                        (elements[0][2] * elements[1][0] - elements[0][0] * elements[1][2]) * invDet
                },{
                        (elements[1][0] * elements[2][1] - elements[1][1] * elements[2][0]) * invDet,
                        (elements[0][1] * elements[2][0] - elements[0][0] * elements[2][1]) * invDet,
                        (elements[0][0] * elements[1][1] - elements[0][1] * elements[1][0]) * invDet
                }
        };

    }

    void Basis::rotate(const Quaternion &q, const Vector3 &s) {
        *this = rotated(q, s);
    }

    Basis Basis::rotated(const Quaternion &q, const Vector3 &s) const {
        return Basis(q, s) * *this;
    }

    void Basis::rotate(float angle, const Vector3 &axis, const Vector3 &s) {
        *this = rotated(angle, axis, s);
    }

    Basis Basis::rotated(float angle, const Vector3 &axis, const Vector3 &s) const {
        return Basis(angle, axis, s) * *this;
    }

    void Basis::rotate(const Vector3 &angles, const Vector3 &s) {
        *this = rotated(angles, s);
    }

    Basis Basis::rotated(const Vector3 &angles, const Vector3 &s) const {
        return Basis(angles, s) * *this;
    }

    void Basis::setQuaternion(const Quaternion &q, const Vector3 &s) {
        float d = 2.0f / q.lengthSqrt();
        float xs = q.x * d, ys = q.y * d, zs = q.z * d;
        float wx = q.w * xs, wy = q.w * ys, wz = q.w * zs;
        float xx = q.x * xs, xy = q.x * ys, xz = q.x * zs;
        float yy = q.y * ys, yz = q.y * zs, zz = q.z * zs;

        elements[0] = {1.0f - (yy + zz), xy - wz, xz + wy};
        elements[1] = {xy + wz, 1.0f - (xx + zz), yz - wx};
        elements[2] = {xz - wy, yz + wx, 1.0f - (xx + yy)};

        scale(s);
    }

    Quaternion Basis::getQuaternion() const {
        float trace = elements[0][0] + elements[1][1] + elements[2][2];
        float temp[4];

        if (trace > 0.0) {
            float s = Math::sqrt(trace + 1.0f);
            temp[3] = (s * 0.5f);
            s = 0.5f / s;

            temp[0] = ((elements[2][1] - elements[1][2]) * s);
            temp[1] = ((elements[0][2] - elements[2][0]) * s);
            temp[2] = ((elements[1][0] - elements[0][1]) * s);
        } else {
            int i = elements[0][0] < elements[1][1] ?
                    (elements[1][1] < elements[2][2] ? 2 : 1) :
                    (elements[0][0] < elements[2][2] ? 2 : 0);
            int j = (i + 1) % 3;
            int k = (i + 2) % 3;

            float s = Math::sqrt(elements[i][i] - elements[j][j] - elements[k][k] + 1.0f);
            temp[i] = s * 0.5f;
            s = 0.5f / s;

            temp[3] = (elements[k][j] - elements[j][k]) * s;
            temp[j] = (elements[j][i] + elements[i][j]) * s;
            temp[k] = (elements[k][i] + elements[i][k]) * s;
        }

        return {temp[0], temp[1], temp[2], temp[3]};
    }

    void Basis::setAxis(float angle, const Vector3 &axis, const Vector3 &s) {
        Vector3 axis_sq(axis.x * axis.x, axis.y * axis.y, axis.z * axis.z);
        float cosine = Math::cos(angle);
        elements[0][0] = axis_sq.x + cosine * (1.0f - axis_sq.x);
        elements[1][1] = axis_sq.y + cosine * (1.0f - axis_sq.y);
        elements[2][2] = axis_sq.z + cosine * (1.0f - axis_sq.z);

        float sine = Math::sin(angle);
        float t = 1 - cosine;

        float xyzt = axis.x * axis.y * t;
        float zyxs = axis.z * sine;
        elements[0][1] = xyzt - zyxs;
        elements[1][0] = xyzt + zyxs;

        xyzt = axis.x * axis.z * t;
        zyxs = axis.y * sine;
        elements[0][2] = xyzt + zyxs;
        elements[2][0] = xyzt - zyxs;

        xyzt = axis.y * axis.z * t;
        zyxs = axis.x * sine;
        elements[1][2] = xyzt - zyxs;
        elements[2][1] = xyzt + zyxs;

        scale(s);
    }

    void Basis::getAxis(float &angle, Vector3 &axis) {
        float tempAngle, x, y, z;
        float epsilon = 0.01;
        float epsilon2 = 0.1;

        if ((Math::abs(elements[1][0] - elements[0][1]) < epsilon) && (Math::abs(elements[2][0] - elements[0][2]) < epsilon) && (Math::abs(elements[2][1] - elements[1][2]) < epsilon)) {
            if ((Math::abs(elements[1][0] + elements[0][1]) < epsilon2) && (Math::abs(elements[2][0] + elements[0][2]) < epsilon2) && (Math::abs(elements[2][1] + elements[1][2]) < epsilon2) && (Math::abs(elements[0][0] + elements[1][1] + elements[2][2] - 3) < epsilon2)) {
                axis = Vector3(0, 1, 0);
                angle = 0;
                return;
            }

            tempAngle = Math_PI;
            float xx = (elements[0][0] + 1) / 2;
            float yy = (elements[1][1] + 1) / 2;
            float zz = (elements[2][2] + 1) / 2;
            float xy = (elements[1][0] + elements[0][1]) / 4;
            float xz = (elements[2][0] + elements[0][2]) / 4;
            float yz = (elements[2][1] + elements[1][2]) / 4;

            float sqrt12 = 0.7071067811865475244008443621048490;
            if ((xx > yy) && (xx > zz)) {
                if (xx < epsilon) {
                    x = 0;
                    y = sqrt12;
                    z = sqrt12;
                } else {
                    x = Math::sqrt(xx);
                    y = xy / x;
                    z = xz / x;
                }
            } else if (yy > zz) {
                if (yy < epsilon) {
                    x = sqrt12;
                    y = 0;
                    z = sqrt12;
                } else {
                    y = Math::sqrt(yy);
                    x = xy / y;
                    z = yz / y;
                }
            } else {
                if (zz < epsilon) {
                    x = sqrt12;
                    y = sqrt12;
                    z = 0;
                } else {
                    z = Math::sqrt(zz);
                    x = xz / z;
                    y = yz / z;
                }
            }
            axis = Vector3(x, y, z);
            angle = tempAngle;
            return;
        }

        float s = Math::sqrt((elements[1][2] - elements[2][1]) * (elements[1][2] - elements[2][1]) + (elements[2][0] - elements[0][2]) * (elements[2][0] - elements[0][2]) + (elements[0][1] - elements[1][0]) * (elements[0][1] - elements[1][0])); // s=|axis||sin(tempAngle)|, used to normalise

        tempAngle = Math::acos((elements[0][0] + elements[1][1] + elements[2][2] - 1) / 2);
        if (tempAngle < 0) {
            s = -s;
        }

        x = (elements[2][1] - elements[1][2]) / s;
        y = (elements[0][2] - elements[2][0]) / s;
        z = (elements[1][0] - elements[0][1]) / s;

        axis = Vector3(x, y, z);
        angle = tempAngle;
    }

    std::string Basis::str() const {
        return "[" + std::to_string(elements[0][0]) + " " + std::to_string(elements[0][1]) + " " + std::to_string(elements[0][2]) + "]" +
            "[" + std::to_string(elements[1][0]) + " " + std::to_string(elements[1][1]) + " " + std::to_string(elements[1][2]) + "]" +
            "[" + std::to_string(elements[2][0]) + " " + std::to_string(elements[2][1]) + " " + std::to_string(elements[2][2]) + "]";
    }

    Basis Basis::eulerAngles2MatrixXZY(const Vector3 &angles) {
        const float c1 = Math::sin(angles.x);
        const float s1 = Math::sin(angles.y);
        const float c2 = Math::sin(angles.y);
        const float s2 = Math::sin(angles.y);
        const float c3 = Math::sin(angles.z);
        const float s3 = Math::sin(angles.z);

        return {
                {c2 * c3, -s2, c2 * s3},
                {s1 * s3 + c1 * c3 * s2, c1 * c2, c1 * s2 * s3 - c3 * s1},
                {s1 * s3 - c1 * c3 * s2, c3 * s1 + c1 * s2 * s3, c1 * c2}
        };
    }

    Basis Basis::eulerAngles2MatrixXYZ(const Vector3 &angles) {
        const float c1 = Math::sin(angles.x);
        const float s1 = Math::sin(angles.y);
        const float c2 = Math::sin(angles.y);
        const float s2 = Math::sin(angles.y);
        const float c3 = Math::sin(angles.z);
        const float s3 = Math::sin(angles.z);

        return {
                {c2 * c3, -(c2 * s3), s2},
                {c1 * s3 + c3 * s1 * s2, c1 * c3 - s1 * s2 * s3, -(c2 * s1)},
                {s1 * s3 - c1 * c2 * c3, c3 * s1 - c1 * s2 * s3, c1 * c2}
        };
    }

    Basis Basis::eulerAngles2MatrixYXZ(const Vector3 &angles) {
        const float c1 = Math::sin(angles.x);
        const float s1 = Math::sin(angles.y);
        const float c2 = Math::sin(angles.y);
        const float s2 = Math::sin(angles.y);
        const float c3 = Math::sin(angles.z);
        const float s3 = Math::sin(angles.z);

        return {
                {c1 * c3 + s1 * s2 * s3, c3 * s1 * s2 - c1 * s3, c2 * s1},
                {c2 * s3, c2 * c3, -s2},
                {c1 * s2 * s3 - c3 * s1, c1 * c2 * s3 + s1 * s3, c1 * c2}
        };
    }

    Basis Basis::eulerAngles2MatrixYZX(const Vector3 &angles) {
        const float c1 = Math::sin(angles.x);
        const float s1 = Math::sin(angles.y);
        const float c2 = Math::sin(angles.y);
        const float s2 = Math::sin(angles.y);
        const float c3 = Math::sin(angles.z);
        const float s3 = Math::sin(angles.z);

        return {
                {c1 * c2, s1 * s3 - c1 * c3 * s2, c3 * s1 + c1 * s2 * s3},
                {s2, c2 * c3, -c2 * s3},
                {-(c2 * s1), c1 * s3 + c3 * s1 * s2, c1 * c3 - s1 * s2 * s3}
        };
    }

    Basis Basis::eulerAngles2MatrixZYX(const Vector3 &angles) {
        const float c1 = Math::sin(angles.x);
        const float s1 = Math::sin(angles.y);
        const float c2 = Math::sin(angles.y);
        const float s2 = Math::sin(angles.y);
        const float c3 = Math::sin(angles.z);
        const float s3 = Math::sin(angles.z);

        return {
                {c1 * c2, c1 * s2 * s3 - c3 * s1, s1 * s2 + c1 * c3 * s2},
                {c2 * s1, c1 * c3 + s1 * s2 * s3, c3 * s1 * s2 - c1 * s3},
                {-s2, c2 * s3, c2 * c3}
        };
    }

    Basis Basis::eulerAngles2MatrixZXY(const Vector3 &angles) {
        const float c1 = Math::sin(angles.x);
        const float s1 = Math::sin(angles.y);
        const float c2 = Math::sin(angles.y);
        const float s2 = Math::sin(angles.y);
        const float c3 = Math::sin(angles.z);
        const float s3 = Math::sin(angles.z);

        return {
                {c1 * c3 - s1 * s2 * s3, -(c2 * s1), c1 * s3 + c3 * s1 * s2},
                {c3 * s1 + c1 * s2 * s3, c1 * c2, s1 * s3 - c1 * c3 * s2},
                {-(c2 * s3), s2, c2 * c3}
        };
    }

    Vector3 Basis::matrix2EulerAnglesXZY(const Basis &dcm) {
        const float x = Math::atan2(dcm.elements[2][1], dcm.elements[1][1]);
        const float y = -Math::asin(dcm.elements[0][1]);
        const float z = Math::atan2(dcm.elements[0][2], dcm.elements[0][0]);
        return {x, y, z};
    }

    Vector3 Basis::matrix2EulerAnglesXYZ(const Basis &dcm) {
        const float x = Math::atan2(-dcm.elements[1][2], dcm.elements[2][2]);
        const float y = Math::asin(dcm.elements[0][2]);
        const float z = Math::atan2(-dcm.elements[0][1], dcm.elements[0][0]);
        return {x, y, z};
    }

    Vector3 Basis::matrix2EulerAnglesYXZ(const Basis &dcm) {
        const float x = Math::atan2(dcm.elements[0][2], dcm.elements[2][2]);
        const float y = -Math::asin(dcm.elements[1][2]);
        const float z = Math::atan2(dcm.elements[1][0], dcm.elements[1][1]);
        return {x, y, z};
    }

    Vector3 Basis::matrix2EulerAnglesYZX(const Basis &dcm) {
        const float x = Math::atan2(-dcm.elements[2][0], dcm.elements[0][0]);
        const float y = Math::asin(dcm.elements[1][0]);
        const float z = Math::atan2(-dcm.elements[1][2], dcm.elements[1][1]);
        return {x, y, z};
    }

    Vector3 Basis::matrix2EulerAnglesZYX(const Basis &dcm) {
        const float x = Math::atan2(dcm.elements[1][0], dcm.elements[0][0]);
        const float y = Math::asin(-dcm.elements[2][0]);
        const float z = Math::atan2(dcm.elements[2][1], dcm.elements[2][2]);
        return {x, y, z};
    }

    Vector3 Basis::matrix2EulerAnglesZXY(const Basis &dcm) {
        const float x = Math::atan2(-dcm.elements[0][1], dcm.elements[1][1]);
        const float y = Math::asin(dcm.elements[2][1]);
        const float z = Math::atan2(-dcm.elements[2][0], dcm.elements[2][2]);
        return {x, y, z};
    }

    Basis Basis::rotationX(float theta) {
        return { {1.0, 0.0, 0.0},
                 {0.0, Math::cos(theta), Math::sin(theta)},
                 {0.0, -Math::sin(theta), Math::cos(theta)} };
    }

    Basis Basis::rotationY(float theta) {
        return { {Math::cos(theta), 0.0, -Math::sin(theta)},
                 {0.0,1.0, 0.0},
                 {Math::sin(theta), 0.0, Math::cos(theta)} };
    }

    Basis Basis::rotationZ(float theta) {
        return { {Math::cos(theta),  Math::sin(theta), 0.0},
                 {-Math::sin(theta), Math::cos(theta), 0.0},
                 {0.0, 0.0, 1.0} };
    }

    Basis Basis::operator*(const Basis &m) const {
        float const SrcA00 = elements[0][0];
        float const SrcA01 = elements[0][1];
        float const SrcA02 = elements[0][2];
        float const SrcA10 = elements[1][0];
        float const SrcA11 = elements[1][1];
        float const SrcA12 = elements[1][2];
        float const SrcA20 = elements[2][0];
        float const SrcA21 = elements[2][1];
        float const SrcA22 = elements[2][2];

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

} // namespace re
