#ifndef RAVENENGINE_VECTOR3_HPP
#define RAVENENGINE_VECTOR3_HPP


#include <string>


namespace re {

    class Vector3 {
    public:
        /**
         * Default constructor
         */
        Vector3();

        /**
         * Constructor for set each axis with the same value
         * @param n Real number
         */
        explicit Vector3(float n);

        /**
         * Constructor from specific values for each axis
         * @param x Value of x-axis
         * @param y Value of y-axis
         * @param z Value of z-axis
         */
        Vector3(float x,float y, float z);

        /**
         * @brief Constructor from C-Style array
         * @param p C-Style array of size 3
         */
        explicit Vector3(const float p[3]);

        /**
         * @brief Copy constructor
         * @param v const reference of Vector3
         */
        Vector3(const Vector3& v);

        /**
         * @brief Copy operator
         * @param v const reference of Vector3
         * @return Vector3 reference
         */
        Vector3& operator=(const Vector3& v);

        /**
         * @brief Random access operator
         * @param index Access index
         * @return Reference of value
         */
        float& operator[](unsigned int index);

        /**
         * @brief Random access operator
         * @param index Access index
         * @return Const reference of value
         */
        const float& operator[](unsigned int index) const;

        Vector3 operator-() const;

        /**
         * @brief Vector addition operator
         * @param n Scalar
         * @return Copy of vector summed by scalar
         */
        Vector3 operator+(float n) const;

        /**
         * @brief Vector addition operator
         * @param v Vector3 object
         * @return Copy of vector summed by other vector
         */
        Vector3 operator+(const Vector3& v) const;

        /**
         * @brief Vector subtraction operator
         * @param n Scalar
         * @return Copy of vector subtracted by scalar
         */
        Vector3 operator-(float n) const;

        /**
         * @brief Vector subtraction operator
         * @param v Vector3 Object
         * @return Copy of vector subtracted by scalar
         */
        Vector3 operator-(const Vector3& v) const;

        /**
         * @brief Scalar multiplication operator
         * @param n Scalar
         * @return Copy of vector scaled by real number
         */
        Vector3 operator*(float n) const;

        /**
         * @brief Vectors dot product
         * @param v Vector3 object
         * @return Dot product of vectors
         */
        float operator*(const Vector3& v) const;

        /**
         * @brief Vector division operations
         * @param n Scalar
         * @return Copy of vector scaled by 1 / n
         */
        Vector3 operator/(float n) const;

        /**
         * @brief Vector addition and assigment operator
         * @param n Scalar
         * @return Reference of this vector
         */
        Vector3& operator+=(float n);

        /**
         * @brief Vector addition and assigment operator
         * @param v Vector3 object
         * @return Reference of this vector
         */
        Vector3& operator+=(const Vector3& v);

        /**
         * @brief Vector subtraction and assigment operator
         * @param n Scalar
         * @return Reference of this vector
         */
        Vector3& operator-=(float n);

        /**
         * @brief Vector subtraction and assigment operator
         * @param v Vector3 object
         * @return Reference of this vector
         */
        Vector3& operator-=(const Vector3& v);

        /**
         * @brief Vector multiplication and assigment operator
         * @param n Scalar
         * @return Reference of this vector
         */
        Vector3& operator*=(float n);

        /**
         * @brief Vector division and assigment operator
         * @param n Scala
         * @return Reference of this vector scaled by 1 / n
         */
        Vector3& operator/=(float n);

        /**
         * @brief Vector comparison bool
         * @param v Vector3 object
         * @return The two vectors are equal
         */
        bool operator==(const Vector3& v) const;

        /**
         * @brief Vector comparison bool
         * @param v Vector3 object
         * @return The two vectors are not equal
         */
        bool operator!=(const Vector3& v) const;

        /**
         *
         * @return Magnitude of vector squared
         */
        [[nodiscard]] float lengthSqrt() const;

        /**
         *
         * @return Magnitude of vector
         */
        [[nodiscard]] float length() const;

        /**
         *
         * @return Magnitude of vector
         */
        [[nodiscard]] Vector3 normal() const;

        /**
         * @brief Normalize this vector
         * @return Reference of this vector
         */
        Vector3& normalize();

        /**
         *
         * @return C-Style array of this vector
         */
        float* ptr();

        /**
         *
         * @return String of this vector
         */
        [[nodiscard]] std::string str() const;

        /**
         *
         * @param v Vector3 object
         * @return Vectors cross product
         */
        [[nodiscard]] Vector3 cross(const Vector3& v) const;

    public:
        float x{}, y{}, z{};
    };

    inline Vector3 operator+(float n, const Vector3& v) {
        return v + n;
    }

    inline Vector3 operator-(float n, const Vector3& v) {
        return v - n;
    }

    inline Vector3 operator*(float n, const Vector3& v) {
        return v * n;
    }

    using vec3 = Vector3;

} // namespace re


#endif //RAVENENGINE_VECTOR3_HPP
