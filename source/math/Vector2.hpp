#ifndef RAVENENGINE_VECTOR2_HPP
#define RAVENENGINE_VECTOR2_HPP


#include <string>
#include <limits>


namespace re {

    class Vector2 {
    public:
        inline Vector2();

        inline explicit Vector2(float n);

        inline Vector2(float x, float y);

        explicit Vector2(const float* v);

        explicit Vector2(const double* v);

        inline Vector2(const Vector2& v);

        [[nodiscard]] inline float lengthSqrt() const;

        [[nodiscard]] float length() const;

        [[nodiscard]] bool isUnit(float tol = std::numeric_limits<float>::epsilon()) const;

        void normalize();

        [[nodiscard]] Vector2 normalized() const;

        [[nodiscard]] inline float dot(const Vector2& v) const;

        inline void inverse();

        [[nodiscard]] inline Vector2 inversed() const;

        [[nodiscard]] std::string str() const;

        inline Vector2& operator=(const Vector2& v);

        inline float& operator[](size_t i);

        inline const float& operator[](size_t i) const;

        inline Vector2 operator-() const;

        inline void operator+=(const Vector2& v);

        inline void operator-=(const Vector2& v);

        inline void operator*=(float s);

        inline void operator*=(const Vector2& v);

        inline void operator/=(float s);

        inline Vector2 operator+(const Vector2& v) const;

        inline Vector2 operator-(const Vector2& v) const;

        inline Vector2 operator*(float s) const;

        inline Vector2 operator*(const Vector2& v) const;

        inline Vector2 operator/(float s) const;

        inline bool operator==(const Vector2& v) const;

        inline bool operator!=(const Vector2& v) const;

        union {
            struct {
                float x, y;
            };
            float values[2]{};
        };
    };

    Vector2::Vector2() = default;

    Vector2::Vector2(float n) : x(n), y(n) {

    }

    Vector2::Vector2(float x, float y) : x(x), y(y) {

    }

    Vector2::Vector2(const Vector2 &v) : x(v.x), y(v.y) {

    }

    /**
     *
     * @return Vector length without apply squared root
     */
    float Vector2::lengthSqrt() const {
        return dot(*this);
    }

    /**
     *
     * @return Dot product(Scalar product) between this vector and another\n
     * @link https://www.wikiwand.com/en/Dot_product.
     */
    float Vector2::dot(const Vector2 &v) const {
        return x * v.x + y * v.y;
    }

    /**
     * Turn this vector in its inverse
     */
    void Vector2::inverse() {
        *this = inversed();
    }

    /**
     *
     * @return Inverse of this vector. 1 divided by each component.
     */
    Vector2 Vector2::inversed() const {
        return {1.0f / x, 1.0f / y};
    }

    Vector2 &Vector2::operator=(const Vector2 &v) {
        x = v.x;
        y = v.y;
    }

    float &Vector2::operator[](size_t i) {
        return values[i];
    }

    const float &Vector2::operator[](size_t i) const {
        return values[i];
    }

    Vector2 Vector2::operator-() const {
        return {-x, -y};
    }

    void Vector2::operator+=(const Vector2 &v) {
        *this = *this + v;
    }

    void Vector2::operator-=(const Vector2 &v) {
        *this = *this - v;
    }

    void Vector2::operator*=(float s) {
        *this = *this * s;
    }

    void Vector2::operator*=(const Vector2 &v) {
        *this = *this * v;
    }

    void Vector2::operator/=(float s) {
        *this = *this / s;
    }

    Vector2 Vector2::operator+(const Vector2 &v) const {
        return {x + v.x, y + v.y};
    }

    Vector2 Vector2::operator-(const Vector2 &v) const {
        return {x - v.x,  y - v.y};
    }

    Vector2 Vector2::operator*(float s) const {
        return {x * s, y * s};
    }

    Vector2 Vector2::operator*(const Vector2 &v) const {
        return {x * v.x,  y * v.y};
    }

    Vector2 Vector2::operator/(float s) const {
        return {x / s, y / s};
    }

    bool Vector2::operator==(const Vector2 &v) const {
        return x == v.x && y == v.y;
    }

    bool Vector2::operator!=(const Vector2 &v) const {
        return x != v.x && y != v.y;
    }

    inline Vector2 operator*(float n, const Vector2& v) {
        return v * n;
    }

    using vec2 = Vector2;


} // namespace re


#endif //RAVENENGINE_VECTOR2_HPP
