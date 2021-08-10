#include "Vector2.hpp"

#include <cmath>


namespace re {

    Vector2::Vector2() = default;

    Vector2::Vector2(float n) : x(n), y(n) {

    }

    Vector2::Vector2(float x, float y) : x(x), y(y) {

    }

    Vector2::Vector2(const float *p) : x(p[0]), y(p[1]){

    }

    Vector2::Vector2(const Vector2 &v) = default;

    Vector2 &Vector2::operator=(const Vector2 &v) = default;

    float &Vector2::operator[](unsigned int index) {
        return (&x)[index];
    }

    const float &Vector2::operator[](unsigned int index) const {
        return (&x)[index];
    }

    Vector2 Vector2::operator+(float n) const {
        return {x + n,  y + n};
    }

    Vector2 Vector2::operator+(const Vector2 &v) const {
        return {x + v.x, y + v.y};
    }

    Vector2 Vector2::operator-(float n) const {
        return {x - n,  y - n};
    }

    Vector2 Vector2::operator-(const Vector2 &v) const {
        return {x - v.x, y - v.y};
    }

    Vector2 Vector2::operator*(float n) const {
        return {x * n,  y * n};
    }

    float Vector2::operator*(const Vector2 &v) const {
        return x * v.x + y * v.y;
    }

    Vector2 Vector2::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
    }

    Vector2 &Vector2::operator+=(float n) {
        return *this = *this + n;
    }

    Vector2 &Vector2::operator+=(const Vector2 &v) {
        return *this = *this + v;
    }

    Vector2 &Vector2::operator-=(float n) {
        return *this = *this - n;
    }

    Vector2 &Vector2::operator-=(const Vector2 &v) {
        return *this = *this - v;
    }

    Vector2 &Vector2::operator*=(float n) {
        return *this = *this * n;
    }

    Vector2 &Vector2::operator/=(float n) {
        return *this = *this / n;
    }

    bool Vector2::operator==(const Vector2 &v) const {
        return x == v.x && y == v.y;
    }

    bool Vector2::operator!=(const Vector2 &v) const {
        return x != v.x && y != v.y;
    }

    float Vector2::lengthSqrt() const {
        return x * x + y * y;
    }

    float Vector2::length() const {
        return std::sqrt(lengthSqrt());
    }

    Vector2 Vector2::normal() const {
        return *this / length();
    }

    Vector2 &Vector2::normalize() {
        return *this = normal();
    }

    float *Vector2::ptr() {
        return (&x);
    }

    std::string Vector2::str() const {
        return "| " + std::to_string(x) + " " + std::to_string(y) + " |";
    }

} // namespace re