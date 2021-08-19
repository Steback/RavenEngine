#include "Vector3.hpp"

#include <cmath>


namespace re {

    Vector3::Vector3() = default;

    Vector3::Vector3(float n) : x(n), y(n), z(n) {

    }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {

    }

    Vector3::Vector3(const float *p) : x(p[0]), y(p[1]), z(p[2]) {

    }

    Vector3::Vector3(const Vector3 &v) = default;

    Vector3 &Vector3::operator=(const Vector3 &v) = default;

    float &Vector3::operator[](unsigned int index) {
        return (&x)[index];
    }

    const float &Vector3::operator[](unsigned int index) const {
        return (&x)[index];
    }

    bool Vector3::operator==(const Vector3 &v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    bool Vector3::operator!=(const Vector3 &v) const {
        return x != v.x && y != v.y && z != v.z;
    }

    Vector3 Vector3::operator-() const {
        return {-x, -y, -z};
    }

    Vector3 Vector3::operator+(float n) const {
        return {x + n, y + n, z + n};
    }

    Vector3 Vector3::operator+(const Vector3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vector3 Vector3::operator-(float n) const {
        return {x - n, y - n, z - n};
    }

    Vector3 Vector3::operator-(const Vector3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vector3 Vector3::operator*(float n) const {
        return {x * n, y * n, z * n};
    }

    float Vector3::operator*(const Vector3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 Vector3::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
    }

    Vector3 &Vector3::operator+=(float n) {
        return *this = *this + n;
    }

    Vector3 &Vector3::operator+=(const Vector3 &v) {
        return *this = *this + v;
    }

    Vector3 &Vector3::operator-=(float n) {
        return *this = *this - n;
    }

    Vector3 &Vector3::operator-=(const Vector3 &v) {
        return *this = *this - v;
    }

    Vector3 &Vector3::operator*=(float n) {
        return *this = *this * n;
    }

    Vector3 &Vector3::operator/=(float n) {
        return *this = *this / n;
    }

    float Vector3::lengthSqrt() const {
        return x * x + y * y + z * z;
    }

    float Vector3::length() const {
        return std::sqrt(lengthSqrt());
    }

    Vector3 Vector3::normal() const {
        return *this / length();
    }

    Vector3 &Vector3::normalize() {
        return *this = normal();
    }

    float *Vector3::ptr() {
        return (&x);
    }

    std::string Vector3::str() const {
        return "| " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " |";
    }

    Vector3 Vector3::cross(const Vector3 &v) const {
        return {
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
        };
    }

} // namespace re