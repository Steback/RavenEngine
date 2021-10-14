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

    /**
     *
     * @param tol
     */
    bool Vector2::isUnit(float tol) const {
        return Math::abs(lengthSqrt() - 1.0f) < 2.0f * tol;
    }

    /**
     * Turn this vector into its unit vector
     */
    void Vector2::normalize() {
        *this = normalized();
    }

    /**
     *
     * @return Unit vector of this vector
     * @link https://www.wikiwand.com/en/Unit_vector
     */
    Vector2 Vector2::normalized() const {
        return *this / length();
    }

    std::string Vector2::str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

} // namespace re