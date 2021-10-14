#include "Quaternion.hpp"

#include "Math.hpp"


namespace re {

    Quaternion::Quaternion(const float *v) {
        w = v[0];
        x = v[1];
        y = v[2];
        z = v[3];
    }

    Quaternion::Quaternion(const double *v) {
        w = static_cast<float >(v[0]);
        x = static_cast<float>(v[1]);
        y = static_cast<float>(v[2]);
        z = static_cast<float >(v[3]);
    }

    float Quaternion::length() const {
        return Math::sqrt(lengthSqrt());
    }

    bool Quaternion::isUnit() const {
        // TODO: Implement Quaternion::isUnit
        return false;
    }

    void Quaternion::normalize() {
        *this = normalized();
    }

    Quaternion Quaternion::normalized() const {
        return *this / length();
    }

    void Quaternion::inverse() {
        *this = inversed();
    }

    Quaternion Quaternion::inversed() const {
        return conjugate() / length();
    }

    Matrix3 Quaternion::getMatrix() const {
        mat3 result(1.0f);
        const float xx = x * x;
        const float yy = y * y;
        const float zz = z * z;
        const float xz = x * z;
        const float xy = x * y;
        const float yz = y * z;
        const float wx = w * x;
        const float wy = w * y;
        const float wz = w * z;

        result[0][0] = 1.0f - 2.0f * (yy + zz);
        result[0][1] = 2.0f * (xy + wz);
        result[0][2] = 2.0f * (xz - wy);

        result[1][0] = 2.0f * (xy - wz);
        result[1][1] = 1.0f - 2.0f * (xx + zz);
        result[1][2] = 2.0f * (yz + wx);

        result[2][0] = 2.0f * (xz + wy);
        result[2][1] = 2.0f * (yz - wx);
        result[2][2] = 1.0f - 2.0f * (xx + yy);
        return result;
    }

    Quaternion Quaternion::eulerAngles2QuatXZY(const Vector3 &angles) {
        const float c1 = Math::cos(0.5f * angles.x);
        const float s1 = Math::sin(0.5f * angles.x);
        const float c2 = Math::cos(0.5f * angles.y);
        const float s2 = Math::sin(0.5f * angles.y);
        const float c3 = Math::cos(0.5f * angles.z);
        const float s3 = Math::sin(0.5f * angles.z);
        const float w = c1 * c2 * c3 - s1 * s2 * s3;
        const float x = c1 * s2 * s3 + s1 * c2 * c3;
        const float y = c1 * c2 * s3 + s1 * s2 * c3;
        const float z = c1 * s2 * c3 - s1 * c2 * s3;
        return {w, x, y, z};
    }

    Quaternion Quaternion::eulerAngles2QuatXYZ(const Vector3 &angles) {
        const float c1 = Math::cos(0.5f * angles.x);
        const float s1 = Math::sin(0.5f * angles.x);
        const float c2 = Math::cos(0.5f * angles.y);
        const float s2 = Math::sin(0.5f * angles.y);
        const float c3 = Math::cos(0.5f * angles.z);
        const float s3 = Math::sin(0.5f * angles.z);
        const float w = c1 * c2 * c3 + s1 * s2 * s3;
        const float x = s1 * c2 * c3 - c1 * s2 * s3;
        const float y = c1 * s2 * c3 + s1 * c2 * s3;
        const float z = c1 * c2 * s3 - s1 * s2 * c3;
        return {w, x, y, z};
    }

    Quaternion Quaternion::eulerAngles2QuatYXZ(const Vector3 &angles) {
        const float c1 = Math::cos(0.5f * angles.x);
        const float s1 = Math::sin(0.5f * angles.x);
        const float c2 = Math::cos(0.5f * angles.y);
        const float s2 = Math::sin(0.5f * angles.y);
        const float c3 = Math::cos(0.5f * angles.z);
        const float s3 = Math::sin(0.5f * angles.z);
        const float w = c1 * c2 * c3 - s1 * s2 * s3;
        const float x = c1 * s2 * c3 - s1 * c2 * s3;
        const float y = c1 * s2 * s3 + s1 * c2 * s3;
        const float z = c1 * c2 * s3 + s1 * s2 * c3;
        return {w, x, y, z};
    }

    Quaternion Quaternion::eulerAngles2QuatYZX(const Vector3 &angles) {
        const float c1 = Math::cos(0.5f * angles.x);
        const float s1 = Math::sin(0.5f * angles.x);
        const float c2 = Math::cos(0.5f * angles.y);
        const float s2 = Math::sin(0.5f * angles.y);
        const float c3 = Math::cos(0.5f * angles.z);
        const float s3 = Math::sin(0.5f * angles.z);
        const float w = c1 * c2 * c3 + s1 * s2 * s3;
        const float x = c1 * c2 * s3 - s1 * s2 * c3;
        const float y = s1 * c2 * c3 - c1 * s2 * s3;
        const float z = c1 * s2 * c3 + s1 * c2 * s3;
        return {w, x, y, z};
    }

    Quaternion Quaternion::eulerAngles2QuatZYX(const Vector3 &angles) {
        const float c1 = Math::cos(0.5f * angles.x);
        const float s1 = Math::sin(0.5f * angles.x);
        const float c2 = Math::cos(0.5f * angles.y);
        const float s2 = Math::sin(0.5f * angles.y);
        const float c3 = Math::cos(0.5f * angles.z);
        const float s3 = Math::sin(0.5f * angles.z);
        const float w = c1 * c2 * c3 - s1 * s2 * s3;
        const float x = c1 * c2 * s3 + s1 * s2 * c3;
        const float y = c1 * s2 * c3 - s1 * c2 * s3;
        const float z = c1 * s2 * s3 + s1 * c2 * c3;
        return {w, x, y, z};
    }

    Quaternion Quaternion::eulerAngles2QuatZXY(const Vector3 &angles) {
        const float c1 = Math::cos(0.5f * angles.x);
        const float s1 = Math::sin(0.5f * angles.x);
        const float c2 = Math::cos(0.5f * angles.y);
        const float s2 = Math::sin(0.5f * angles.y);
        const float c3 = Math::cos(0.5f * angles.z);
        const float s3 = Math::sin(0.5f * angles.z);
        const float w = c1 * c2 * c3 - s1 * s2 * s3;
        const float x = c1 * c2 * s3 + s1 * s2 * c3;
        const float y = c1 * s2 * c3 - s1 * c2 * s3;
        const float z = c1 * s2 * s3 + s1 * c2 * c3;
        return {w, x, y, z};
    }

    Vector3 Quaternion::quat2EulerAnglesXZY(const Quaternion &quat) {
        const float ww   = quat.w * quat.w;
        const float xx   = quat.x * quat.x;
        const float yy   = quat.y * quat.y;
        const float zz   = quat.z * quat.z;
        const float xy2 = 2.0f * quat.x * quat.y;
        const float wz2 = 2.0f * quat.w * quat.z;
        const float xz2 = 2.0f * quat.x * quat.z;
        const float wy2 = 2.0f * quat.w * quat.y;
        const float yz2 = 2.0f * quat.y * quat.z;
        const float wx2 = 2.0f * quat.w * quat.x;
        const float m11 = ww + xx - yy - zz;
        const float m12 = xy2 + wz2;
        const float m13 = xz2 - wy2;
        const float m22 = ww - xx + yy + zz;
        const float m32 = yz2 - wx2;
        const float phi   = Math::atan2(-m32, m22);
        const float theta = Math::asin(m12);
        const float psi   = Math::atan2(-m13, m11);
        return {phi, theta, psi};
    }

    Vector3 Quaternion::quat2EulerAnglesXYZ(const Quaternion &quat) {
        const float ww   = quat.w * quat.w;
        const float xx   = quat.x * quat.x;
        const float yy   = quat.y * quat.y;
        const float zz   = quat.z * quat.z;
        const float xy2 = 2.0f * quat.x * quat.y;
        const float wz2 = 2.0f * quat.w * quat.z;
        const float xz2 = 2.0f * quat.x * quat.z;
        const float wy2 = 2.0f * quat.w * quat.y;
        const float yz2 = 2.0f * quat.y * quat.z;
        const float wx2 = 2.0f * quat.w * quat.x;
        const float m11 = ww + xx - yy - zz;
        const float m12 = xy2 + wz2;
        const float m13 = xz2 - wy2;
        const float m23 = yz2 + wx2;
        const float m33 = ww - xx - yy + zz;
        const float phi   = Math::atan2(m23, m33);
        const float theta = -Math::asin(m13);
        const float psi   = Math::atan2(m12, m11);
        return {phi, theta, psi};
    }

    Vector3 Quaternion::quat2EulerAnglesYXZ(const Quaternion &quat) {
        const float ww   = quat.w * quat.w;
        const float xx   = quat.x * quat.x;
        const float yy   = quat.y * quat.y;
        const float zz   = quat.z * quat.z;
        const float xy2 = 2.0f * quat.x * quat.y;
        const float wz2 = 2.0f * quat.w * quat.z;
        const float xz2 = 2.0f * quat.x * quat.z;
        const float wy2 = 2.0f * quat.w * quat.y;
        const float yz2 = 2.0f * quat.y * quat.z;
        const float wx2 = 2.0f * quat.w * quat.x;
        const float m13 = xz2 - wy2;
        const float m21 = xy2 - wz2;
        const float m22 = ww - xx + yy + zz;
        const float m23 = yz2 + wx2;
        const float m33 = ww - xx - yy + zz;
        const float phi   = Math::atan2(-m13, m33);
        const float theta = Math::asin(m23);
        const float psi   = Math::atan2(-m21, m22);
        return {phi, theta, psi};
    }

    Vector3 Quaternion::quat2EulerAnglesYZX(const Quaternion &quat) {
        const float ww   = quat.w * quat.w;
        const float xx   = quat.x * quat.x;
        const float yy   = quat.y * quat.y;
        const float zz   = quat.z * quat.z;
        const float xy2 = 2.0f * quat.x * quat.y;
        const float wz2 = 2.0f * quat.w * quat.z;
        const float xz2 = 2.0f * quat.x * quat.z;
        const float wy2 = 2.0f * quat.w * quat.y;
        const float yz2 = 2.0f * quat.y * quat.z;
        const float wx2 = 2.0f * quat.w * quat.x;
        const float m11 = ww + xx - yy - zz;
        const float m21 = xy2 - wz2;
        const float m22 = ww - xx + yy + zz;
        const float m23 = yz2 + wx2;
        const float m31 = xz2 + wy2;
        const float phi   = Math::atan2(m31, m11);
        const float theta = -Math::asin(m21);
        const float psi   = Math::atan2(m23, m22);
        return {phi, theta, psi};
    }

    Vector3 Quaternion::quat2EulerAnglesZYX(const Quaternion &quat) {
        const float ww   = quat.w * quat.w;
        const float xx   = quat.x * quat.x;
        const float yy   = quat.y * quat.y;
        const float zz   = quat.z * quat.z;
        const float xy2 = 2.0f * quat.x * quat.y;
        const float wz2 = 2.0f * quat.w * quat.z;
        const float xz2 = 2.0f * quat.x * quat.z;
        const float wy2 = 2.0f * quat.w * quat.y;
        const float yz2 = 2.0f * quat.y * quat.z;
        const float wx2 = 2.0f * quat.w * quat.x;
        const float m11 = ww + xx - yy - zz;
        const float m21 = xy2 - wz2;
        const float m31 = xz2 + wy2;
        const float m32 = yz2 - wx2;
        const float m33 = ww - xx - yy + zz;
        const float phi   = Math::atan2(-m21, m11);
        const float theta = Math::asin(m31);
        const float psi   = Math::atan2(-m32, m33);
        return {phi, theta, psi};
    }

    Vector3 Quaternion::quat2EulerAnglesZXY(const Quaternion &quat) {
        const float ww   = quat.w * quat.w;
        const float xx   = quat.x * quat.x;
        const float yy   = quat.y * quat.y;
        const float zz   = quat.z * quat.z;
        const float xy2 = 2.0f * quat.x * quat.y;
        const float wz2 = 2.0f * quat.w * quat.z;
        const float xz2 = 2.0f * quat.x * quat.z;
        const float wy2 = 2.0f * quat.w * quat.y;
        const float yz2 = 2.0f * quat.y * quat.z;
        const float wx2 = 2.0f * quat.w * quat.x;
        const float m12 = xy2 + wz2;
        const float m22 = ww - xx + yy + zz;
        const float m31 = xz2 + wy2;
        const float m32 = yz2 - wx2;
        const float m33 = ww - xx - yy + zz;
        const float phi   = Math::atan2(m12, m22);
        const float theta = -Math::asin(m32);
        const float psi   = Math::atan2(m31, m33);
        return {phi, theta, psi};
    }

    std::string Quaternion::str() const {
        return "(" + std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

} // namespace re