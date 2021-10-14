#ifndef RAVENENGINE_VECTOR3_HPP
#define RAVENENGINE_VECTOR3_HPP


#include <string>


namespace re {

    class Vector3 {
    public:
        inline Vector3();

        inline explicit Vector3(float n);

        inline Vector3(float x, float y, float z);

        explicit Vector3(const float* v);

        explicit Vector3(const double * v);

        inline Vector3(const Vector3& v);

        [[nodiscard]] inline float lengthSqrt() const;

        [[nodiscard]] float length() const;

        [[nodiscard]] bool isUnit(float tol = std::numeric_limits<float>::epsilon()) const;

        void normalize();

        [[nodiscard]] Vector3 normalized() const;

        [[nodiscard]] inline float dot(const Vector3& v) const;

        [[nodiscard]] inline Vector3 cross(const Vector3& v) const;

        inline void inverse();

        [[nodiscard]] inline Vector3 inversed() const;

        [[nodiscard]] std::string str() const;

        inline Vector3& operator=(const Vector3& v);

        inline float& operator[](size_t i);

        inline const float& operator[](size_t i) const;

        inline Vector3 operator-() const;

        inline void operator+=(const Vector3& v);

        inline void operator-=(const Vector3& v);

        inline void operator*=(float s);

        inline void operator*=(const Vector3& v);

        inline void operator/=(float s);

        inline Vector3 operator+(const Vector3& v) const;

        inline Vector3 operator-(const Vector3& v) const;

        inline Vector3 operator*(float s) const;

        inline Vector3 operator*(const Vector3& v) const;

        inline Vector3 operator/(float s) const;

        inline bool operator==(const Vector3& v) const;

        inline bool operator!=(const Vector3& v) const;

    public:
        union {
            struct {
                float x, y, z;
            };
            float values[3]{};
        };
    };

    Vector3::Vector3() = default;

    Vector3::Vector3(float n) : x(n), y(n), z(n) {

    }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {

    }

    Vector3::Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {

    }

    /**
     *
     * @return Vector length without apply squared root
     */
    float Vector3::lengthSqrt() const {
        return dot(*this);
    }

    /**
     *
     * @return Dot product(Scalar product) between this vector and another\n
     * @link https://www.wikiwand.com/en/Dot_product.
     */
    float Vector3::dot(const Vector3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     *
     * @return Cross product between this vector and another\n
     * @link https://www.wikiwand.com/en/Cross_product
     */
    Vector3 Vector3::cross(const Vector3 &v) const {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }

    /**
     * Turn this vector in its inverse
     */
    void Vector3::inverse() {
        *this = inversed();
    }

    /**
     *
     * @return Inverse of this vector. 1 divided by each component.
     */
    Vector3 Vector3::inversed() const {
        return {1.0f / x, 1.0f / y, 1.0f / z};
    }

    Vector3 &Vector3::operator=(const Vector3 &v) {
        x = v.x; y = v.y, z = v.z;
    }

    float &Vector3::operator[](size_t i) {
        return values[i];
    }

    const float &Vector3::operator[](size_t i) const {
        return values[i];
    }

    Vector3 Vector3::operator-() const {
        return {-x, -y, -z};
    }

    void Vector3::operator+=(const Vector3 &v) {
        *this = *this + v;
    }

    void Vector3::operator-=(const Vector3 &v) {
        *this = *this - v;
    }

    void Vector3::operator*=(float s) {
        *this = *this * s;
    }

    void Vector3::operator*=(const Vector3 &v) {
        *this = *this * v;
    }

    void Vector3::operator/=(float s) {
        *this = *this / s;
    }

    Vector3 Vector3::operator+(const Vector3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vector3 Vector3::operator-(const Vector3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vector3 Vector3::operator*(float s) const {
        return {x * s, y * s, z * s};
    }

    Vector3 Vector3::operator*(const Vector3 &v) const {
        return {x * v.x, y * v.y, z * v.z};
    }

    Vector3 Vector3::operator/(float s) const {
        return {x / s, y / s, z / s};
    }

    bool Vector3::operator==(const Vector3 &v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    bool Vector3::operator!=(const Vector3 &v) const {
        return x != v.x && y != v.y && z != v.z;
    }

    inline Vector3 operator*(float n, const Vector3& v) {
        return v * n;
    }

    using vec3 = Vector3;

} // namespace re


#endif //RAVENENGINE_VECTOR3_HPP
