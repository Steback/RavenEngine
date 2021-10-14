#include "Vector4.hpp"

#include "Math.hpp"


namespace re {

    Vector4::Vector4(const float *v) {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    Vector4::Vector4(const double *v) {
        x = static_cast<float>(v[0]);
        y = static_cast<float>(v[1]);
        z = static_cast<float>(v[2]);
        w = static_cast<float >(v[3]);
    }

    float Vector4::length() const {
        return Math::sqrt(lengthSqrt());
    }

    bool Vector4::isUnit(float tol) const {
        return Math::abs(lengthSqrt() - 1.0f) < 2.0f * tol;
    }

    void Vector4::normalize() {
        *this = normalized();
    }

    Vector4 Vector4::normalized() const {
        return *this / length();
    }

    std::string Vector4::str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }

} // namespace re