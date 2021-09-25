#include "Quaternion.hpp"

#include <cmath>
#include <functional>

#include "Common.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"


namespace re {

    Quaternion::Quaternion() = default;

    Quaternion::Quaternion(float s, float i, float j, float k) : w(s), x(i), y(j), z(k) {

    }

    Quaternion::Quaternion(float s, const Vector3 &v) : w(s), x(v.x), y(v.y), z(v.z) {

    }

    Quaternion::Quaternion(const Vector3 &eulerAngles) {
        vec3 c = cos(eulerAngles * 0.5f);
        vec3 s = sin(eulerAngles * 0.5f);

        w = c.x * c.y * c.z + s.x * s.y * s.z;
        x = s.x * c.y * c.z - c.x * s.y * s.z;
        y = c.x * s.y * c.z + s.x * c.y * s.z;
        z = c.x * c.y * s.z - s.x * s.y * c.z;
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

    Matrix3 Quaternion::rotationMatrix() const {
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
        return {pitch(), yaw(), roll()};
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

    float Quaternion::roll() const {
        return std::atan2(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z);
    }

    float Quaternion::pitch() const {
        const float y_ = 2.0f * (y * z + w * x);
        const float x_ = w * w - x * x - y * y + z * z;

        if (vec2(x_, y_) == vec2())
            return 2.0f * std::atan2(x, w);

        return std::atan2(y_, x_);
    }

    float Quaternion::yaw() const {
        return std::asin(std::clamp(-2.0f * (x * z - w * y), -1.0f, 1.0f));
    }

} // namespace re