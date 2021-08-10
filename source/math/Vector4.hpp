#ifndef RAVENENGINE_VECTOR4_HPP
#define RAVENENGINE_VECTOR4_HPP


#include <string>


namespace re {

    class Vector4 {
    public:
        /**
         * Default constructor
         */
        Vector4();

        /**
         * Constructor for set each axis with the same value
         * @param n Real number
         */
        explicit Vector4(float n);

        /**
         * Constructor from specific values for each axis
         * @param x Value of x-axis
         * @param y Value of y-axis
         * @param z Value of z-axis
         * @param w Value of w
         */
        Vector4(float x, float y, float z, float w);

        /**
         * @brief Constructor from C-Style array
         * @param p C-Style array of size 4
         */
        explicit Vector4(const float p[4]);

        /**
         * @brief Copy constructor
         * @param v const reference of Vector3
         */
        Vector4(const Vector4& v);

        /**
         * @brief Copy operator
         * @param v const reference of Vector3
         * @return Vector4 reference
         */
        Vector4& operator=(const Vector4& v);

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

        /**
         * @brief Vector addition operator
         * @param n Scalar
         * @return Copy of vector summed by scalar
         */
        Vector4 operator+(float n) const;

        /**
         * @brief Vector addition operator
         * @param v Vector4 object
         * @return Copy of vector summed by other vector
         */
        Vector4 operator+(const Vector4& v) const;

        /**
         * @brief Vector subtraction operator
         * @param n Scalar
         * @return Copy of vector subtracted by scalar
         */
        Vector4 operator-(float n) const;

        /**
         * @brief Vector subtraction operator
         * @param v Vector4 Object
         * @return Copy of vector subtracted by scalar
         */
        Vector4 operator-(const Vector4& v) const;

        /**
         * @brief Scalar multiplication operator
         * @param n Scalar
         * @return Copy of vector scaled by real number
         */
        Vector4 operator*(float n) const;

        /**
         * @brief Vectors dot product
         * @param v Vector4 object
         * @return Dot product of vectors
         */
        float operator*(const Vector4& v) const;

        /**
         * @brief Vector division operations
         * @param n Scalar
         * @return Copy of vector scaled by 1 / n
         */
        Vector4 operator/(float n) const;

        /**
         * @brief Vector addition and assigment operator
         * @param n Scalar
         * @return Reference of this vector
         */
        Vector4& operator+=(float n);

        /**
         * @brief Vector addition and assigment operator
         * @param v Vector4 object
         * @return Reference of this vector
         */
        Vector4& operator+=(const Vector4& v);

        /**
         * @brief Vector subtraction and assigment operator
         * @param n Scalar
         * @return Reference of this vector
         */
        Vector4& operator-=(float n);

        /**
         * @brief Vector subtraction and assigment operator
         * @param v Vector4 object
         * @return Reference of this vector
         */
        Vector4& operator-=(const Vector4& v);

        /**
         * @brief Vector multiplication and assigment operator
         * @param n Scalar
         * @return Reference of this vector
         */
        Vector4& operator*=(float n);

        /**
         * @brief Vector division and assigment operator
         * @param n Scala
         * @return Reference of this vector scaled by 1 / n
         */
        Vector4& operator/=(float n);

        /**
         * @brief Vector comparison bool
         * @param v Vector4 object
         * @return The two vectors are equal
         */
        bool operator==(const Vector4& v) const;

        /**
         * @brief Vector comparison bool
         * @param v Vector4 object
         * @return The two vectors are not equal
         */
        bool operator!=(const Vector4& v) const;

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
        [[nodiscard]] Vector4 normal() const;

        /**
         * @brief Normalize this vector
         * @return Reference of this vector
         */
        Vector4& normalize();

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

    public:
        float x{}, y{}, z{}, w{};
    };

    inline Vector4 operator+(float n,  const Vector4& v) {
        return v + n;
    }

    inline Vector4 operator-(float n,  const Vector4& v) {
        return v - n;
    }

    inline Vector4 operator*(float n,  const Vector4& v) {
        return v * n;
    }

    using vec4 = Vector4;


} // namespace re


#endif //RAVENENGINE_VECTOR4_HPP
