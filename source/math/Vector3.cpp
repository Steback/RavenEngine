#include "Vector3.hpp"

#include "Math.hpp"


namespace re {

    Vector3::Vector3(const float *v) {
        x = v[0];
        y = v[1];
        z = v[2];
    }

    Vector3::Vector3(const double *v) {
        x = static_cast<float >(v[0]);
        y = static_cast<float >(v[1]);
        z = static_cast<float >(v[2]);
    }

    float Vector3::length() const {
        return Math::sqrt(lengthSqrt());
    }

    bool Vector3::isUnit() const {
        // TODO: Implement Vector3::isUnit
        return false;
    }

    void Vector3::normalize() {
        *this = normalized();
    }

    Vector3 Vector3::normalized() const {
        return *this / length();
    }

    std::string Vector3::str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

} // namespace re