#include "Vector2.hpp"

#include "Math.hpp"


namespace re {

    Vector2::Vector2(const float *v) {
        x = v[0];
        y = v[1];
    }

    Vector2::Vector2(const double *v) {
        x = static_cast<float>(v[0]);
        y = static_cast<float>(v[1]);
    }

    float Vector2::length() const {
        return Math::sqrt(lengthSqrt());
    }

    bool Vector2::isUnit() const {
        // TODO: Implement Vector2::isUnit
        return false;
    }

    void Vector2::normalize() {
        *this = normalized();
    }

    Vector2 Vector2::normalized() const {
        return *this / length();
    }

    std::string Vector2::str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

} // namespace re