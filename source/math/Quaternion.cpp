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
        float cz = std::cos(eulerAngles.z * 0.5f);
        float sz = std::sin(eulerAngles.z * 0.5f);
        float cy = std::cos(eulerAngles.y * 0.5f);
        float sy = std::sin(eulerAngles.y * 0.5f);
        float cx = std::cos(eulerAngles.x * 0.5f);
        float sx = std::sin(eulerAngles.x * 0.5f);

        w = cx * cy * cz + sx * sy * sz;
        x = sx * cy * cz - cx * sy * sz;
        y = cx * sy * cz + sx * cy * sz;
        z = cx * cy * sz - sx * sy * cz;
    }

    Quaternion::Quaternion(const float *p) : w(p[0]), x(p[1]), y(p[2]), z(p[3]) {

    }

    Quaternion::Quaternion(const double *p) {
        w = static_cast<float>(p[0]);
        x = static_cast<float>(p[1]);
        y = static_cast<float>(p[2]);
        z = static_cast<float>(p[3]);
    }

    Quaternion::Quaternion(const Quaternion &q) = default;

    Quaternion &Quaternion::operator=(const Quaternion &q) = default;

    float &Quaternion::operator[](size_t i) {
        return (&w)[i];
    }

    const float &Quaternion::operator[](size_t i) const {
        return (&w)[i];
    }

    bool Quaternion::operator==(const Quaternion &q) const {
        return w == q.w && x == q.x && y == q.y && z == q.z;
    }

    bool Quaternion::operator!=(const Quaternion &q) const {
        return w != q.w && x != q.x && y != q.y && z != q.z;
    }

    Quaternion Quaternion::operator-() const {
        return {-w, -x, -y, -z};
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
        return {w / n, x / n, y / n, z / n};
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
        mat3 result(1.0f);
        float qxx = x * x;
        float qyy = y * y;
        float qzz = z * z;
        float qxz = x * z;
        float qxy = x * y;
        float qyz = y * z;
        float qwx = w * x;
        float qwy = w * y;
        float qwz = w * z;

        result[0][0] = 1.0f - 2.0f * (qyy +  qzz);
        result[0][1] = 2.0f * (qxy + qwz);
        result[0][2] = 2.0f * (qxz - qwy);

        result[1][0] = 2.0f * (qxy - qwz);
        result[1][1] = 1.0f - 2.0f * (qxx +  qzz);
        result[1][2] = 2.0f * (qyz + qwx);

        result[2][0] = 2.0f * (qxz + qwy);
        result[2][1] = 2.0f * (qyz - qwx);
        result[2][2] = 1.0f - 2.0f * (qxx +  qyy);

        return result;
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