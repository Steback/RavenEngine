#ifndef RAVENENGINE_VECTOR4_HPP
#define RAVENENGINE_VECTOR4_HPP


#include <string>
#include <limits>


namespace re {

    class Vector4 {
    public:
        inline Vector4();

        inline explicit Vector4(float n);

        inline Vector4(float x, float y, float z, float w);

        explicit Vector4(const float* v);

        explicit Vector4(const double* v);

        inline Vector4(const Vector4& v);

        [[nodiscard]] inline float lengthSqrt() const;

        [[nodiscard]] float length() const;

        [[nodiscard]] bool isUnit(float tol = std::numeric_limits<float>::epsilon()) const;

        void normalize();

        [[nodiscard]] Vector4 normalized() const;

        [[nodiscard]] inline float dot(const Vector4& v) const;

        inline void inverse();

        [[nodiscard]] inline Vector4 inversed() const;

        [[nodiscard]] std::string str() const;

        inline Vector4& operator=(const Vector4&) = default;

        inline Vector4& operator=(Vector4&&) = default;

        inline float& operator[](size_t i);

        inline const float& operator[](size_t i) const;

        inline Vector4 operator-() const;

        inline void operator+=(const Vector4& v);

        inline void operator-=(const Vector4& v);

        inline void operator*=(float s);

        inline void operator*=(const Vector4& v);

        inline void operator/=(float s);

        inline Vector4 operator+(const Vector4& v) const;

        inline Vector4 operator-(const Vector4& v) const;

        inline Vector4 operator*(float s) const;

        inline Vector4 operator*(const Vector4& v) const;

        inline Vector4 operator/(float s) const;

        inline bool operator==(const Vector4& v) const;

        inline bool operator!=(const Vector4& v) const;

        union {
            struct {
                float x, y, z, w;
            };
            float values[4]{};
        };
    };

    Vector4::Vector4() {

    }

    Vector4::Vector4(float n) : x(n), y(n), z(n), w(n) {

    }

    Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {

    }

    Vector4::Vector4(const Vector4 &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    /**
     *
     * @return Vector length without apply squared root
     */
    float Vector4::lengthSqrt() const {
        return dot(*this);
    }

    /**
     *
     * @return Dot product(Scalar product) between this vector and another\n
     * @link https://www.wikiwand.com/en/Dot_product.
     */
    float Vector4::dot(const Vector4 &v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    /**
     * Turn this vector in its inverse
     */
    void Vector4::inverse() {
        *this = inversed();
    }

    /**
     *
     * @return Inverse of this vector. 1 divided by each component.
     */
    Vector4 Vector4::inversed() const {
        return {-x, -y, -z, -w};
    }

    float &Vector4::operator[](size_t i) {
        return values[i];
    }

    const float &Vector4::operator[](size_t i) const {
        return values[i];
    }

    Vector4 Vector4::operator-() const {
        return {-x, -y, -z, -w};
    }

    void Vector4::operator+=(const Vector4 &v) {
        *this = *this + v;
    }

    void Vector4::operator-=(const Vector4 &v) {
        *this = *this - v;
    }

    void Vector4::operator*=(float s) {
        *this = *this * s;
    }

    void Vector4::operator*=(const Vector4 &v) {
        *this = *this * v;
    }

    void Vector4::operator/=(float s) {
        *this = *this / s;
    }

    Vector4 Vector4::operator+(const Vector4 &v) const {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    Vector4 Vector4::operator-(const Vector4 &v) const {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    Vector4 Vector4::operator*(float s) const {
        return {x * s, y * s, z * s, w * s};
    }

    Vector4 Vector4::operator*(const Vector4 &v) const {
        return {x * v.x, y * v.y, z * v.z, w * v.w};
    }

    Vector4 Vector4::operator/(float s) const {
        return {x / s, y / s, z / s, w / s};
    }

    bool Vector4::operator==(const Vector4 &v) const {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    bool Vector4::operator!=(const Vector4 &v) const {
        return x != v.x && y != v.y && z != v.z && w != v.w;
    }

    inline Vector4 operator*(float s, const Vector4& v) {
        return v * s;
    }

    using vec4 = Vector4;

} // namespace re


#endif //RAVENENGINE_VECTOR4_HPP
