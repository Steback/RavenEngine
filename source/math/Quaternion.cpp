#include "Quaternion.hpp"

#include <cmath>

#include "Common.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"


namespace re {

    Quaternion::Quaternion() = default;

    Quaternion::Quaternion(float s, float i, float j, float k) : w(s), x(i), y(j), z(k) {

    }

    Quaternion::Quaternion(float s, const Vector3 &v) : w(s), x(v.x), y(v.y), z(v.z) {

    }

    Quaternion::Quaternion(const Vector3 &eulerAngles) {
        float cy = std::cos(eulerAngles.z * 0.5f);
        float sy = std::sin(eulerAngles.z * 0.5f);
        float cp = std::cos(eulerAngles.y * 0.5f);
        float sp = std::sin(eulerAngles.y * 0.5f);
        float cr = std::cos(eulerAngles.x * 0.5f);
        float sr = std::sin(eulerAngles.x * 0.5f);

        w = cr * cp * cy + sr * sp * sy;
        x = sr * cp * cy - cr * sp * sy;
        y = cr * sp * cy + sr * cp * sy;
        z = cr * cp * sy - sr * sp * cy;
    }

    Quaternion Quaternion::operator+(float n) const {
        return {w + n, x + n, y + n, z + n};
    }

    Quaternion Quaternion::operator+(const Quaternion &q) const {
        return {w + q.w, x + q.x, y + q.y, z + q.z};
    }

    Quaternion Quaternion::operator-(float n) const {
        return {w - n, x - n, y - n, z - n};
    }

    Quaternion Quaternion::operator-(const Quaternion &q) const {
        return {w - q.w, x - q.x, y - q.y, z - q.z};
    }

    Quaternion Quaternion::operator*(float n) const {
        return {w * n, x * n, y * n, z * n};
    }

    Quaternion Quaternion::operator*(const Quaternion &q) const {
        return {
                w * q.w - x * q.x - y * q.y - z * q.z,
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + x * q.w
        };
    }

    Quaternion Quaternion::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
    }

    Quaternion &Quaternion::operator+=(float n) {
        return *this = *this + n;
    }

    Quaternion &Quaternion::operator+=(const Quaternion &q) {
        return *this = *this + q;
    }

    Quaternion &Quaternion::operator-=(float n) {
        return *this = *this - n;
    }

    Quaternion &Quaternion::operator-=(const Quaternion &q) {
        return *this = *this - q;
    }

    Quaternion &Quaternion::operator*=(float n) {
        return *this = *this * n;
    }

    Quaternion &Quaternion::operator*=(const Quaternion &q) {
        return *this = *this * q;
    }

    Quaternion &Quaternion::operator/=(float n) {
        return *this = *this / n;
    }

    Vector3 Quaternion::getVectorPart() const {
        return {x, y, z};
    }

    Matrix3 Quaternion::getRotationMatrix() const {
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        return {
                { 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy) },
                { 2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx) },
                { 2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2  + y2) }
        };
    }

    void Quaternion::setRotationMatrix(const Matrix3 &m) {
        float m00 = m[0][0];
        float m11 = m[1][1];
        float m22 = m[2][2];
        float sum = m00 + m11 + m22;

        if (sum > 0.0f) {
            w = std::sqrt(sum + 1.0f) * 0.5f;

            float f = 0.25f / w;

            x = (m[2][1] - m[1][2]) * f;
            y = (m[0][2] - m[2][0]) * f;
            z = (m[1][0] - m[0][1]) * f;
        } else if ((m00 > m11) && (m00 > m22)) {
            x = std::sqrt(m00 - m11 - m22 + 1.0f) * 0.5f;

            float f = 0.25f / x;

            w = (m[2][1] - m[1][2]) * f;
            y = (m[1][0] + m[0][1]) * f;
            z = (m[0][2] + m[2][0]) * f;
        } else if (m11 > m22) {
            y = std::sqrt(m11 - m00 - m22 + 1.0f) * 0.5f;

            float f = 0.25f / y;

            w = (m[0][2] - m[2][0]) * f;
            x = (m[1][0] + m[0][1]) * f;
            z = (m[2][1] + m[1][2]) * f;
        } else {
            z = std::sqrt(m22 - m00 - m11 + 1.0f) * 0.5f;

            float f = 0.25f / z;

            w = (m[1][0] - m[0][1]) * f;
            x = (m[0][2] + m[2][0]) * f;
            x = (m[2][1] + m[1][2]) * f;
        }
    }

    Vector3 Quaternion::getEulerAngles() const {
        Vector3 angles;

        // roll (x-axis rotation)
        float sinr_cosp = 2 * (w * x + y * z);
        float cosr_cosp = 1 - 2 * (x * x + y * y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        float sinp = 2 * (w * y - z * x);
        if (std::abs(sinp) >= 1)
            angles.y = std::copysign(pi() / 2, sinp); // use 90 degrees if out of range
        else
            angles.y = std::asin(sinp);

        // yaw (z-axis rotation)
        float siny_cosp = 2 * (w * z + x * y);
        float cosy_cosp = 1 - 2 * (y * y + z * z);
        angles.x = std::atan2(siny_cosp, cosy_cosp);

        return angles;
    }

    Quaternion Quaternion::conjugate() const {
        return {w, -x, -y, -z};
    }

    float Quaternion::norm() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    void Quaternion::normalise() {
        *this /= norm();
    }

    Quaternion Quaternion::inverse() const {
        return conjugate() / norm();
    }

    Quaternion Quaternion::unit() const {
        return *this / norm();
    }

    float Quaternion::dot(const Quaternion &q) const {
        return w * q.w + x * q.x + y * q.y + z * q.z;
    }

    float *Quaternion::ptr() {
        return (&w);
    }

    std::string Quaternion::str() const {
        return "| " + std::to_string(w) + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " |";
    }

} // namespace re