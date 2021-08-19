#ifndef RAVENENGINE_VECTOR2_HPP
#define RAVENENGINE_VECTOR2_HPP


#include <string>


namespace re {

    /**
     * @brief Vector 2D class
     */
    class Vector2 {
    public:
        /**
         * @brief Default constructor
         */
        Vector2();

        /**
         * Constructor for set each axis with the same value
         * @param n Real number
         */
        explicit Vector2(float n);

        /**
         * Constructor from specific values for each axis
         * @param x Value of x-axis
         * @param y Value of y-axis
         */
        Vector2(float x, float y);

        /**
         * @brief Constructor from C-Style array
         * @param p C-Style array of size 2
         */
        explicit Vector2(const float p[2]);

        /**
         * @brief Copy constructor
         * @param v const reference of Vector2
         */
        Vector2(const Vector2& v);

        /**
         * @brief Copy operator
         * @param v const reference of Vector2
         * @return Vector2 reference
         */
        Vector2& operator=(const Vector2& v);

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

        bool operator==(const Vector2& v) const;

        bool operator!=(const Vector2& v) const;

        Vector2 operator-() const;

        Vector2 operator+(float n) const;

        Vector2 operator+(const Vector2& v) const;

        Vector2 operator-(float n) const;

        Vector2 operator-(const Vector2& v) const;

        Vector2 operator*(float n) const;

        /**
         * @brief Vectors dot product
         * @param v Vector2 object
         * @return Dot product of vectors
         */
        float operator*(const Vector2& v) const;

        Vector2 operator/(float n) const;

        Vector2& operator+=(float n);

        Vector2& operator+=(const Vector2& v);

        Vector2& operator-=(float n);

        Vector2& operator-=(const Vector2& v);

        Vector2& operator*=(float n);

        Vector2& operator/=(float n);

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
         * @return Normal vector
         */
        [[nodiscard]] Vector2 normal() const;

        /**
         * @brief Normalize this vector
         * @return Reference of this vector
         */
        Vector2& normalize();

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
        float x{}, y{};
    };

    inline Vector2 operator+(float n, const Vector2& v) {
        return  v + n;
    }

    inline Vector2 operator-(float n, const Vector2& v) {
        return  v - n;
    }

    inline Vector2 operator*(float n, const Vector2& v) {
        return  v * n;
    }

    using vec2 = Vector2;


} // namespace re


#endif //RAVENENGINE_VECTOR2_HPP
