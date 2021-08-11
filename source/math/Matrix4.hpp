#ifndef RAVENENGINE_MATRIX4_HPP
#define RAVENENGINE_MATRIX4_HPP


#include <string>

#include "Vector4.hpp"


namespace re {

    class Matrix4 {
    public:
        /**
         * @brief Default Constructor
         */
        Matrix4();

        /**
         * Constructor for set each axis with the same value
         * @param n Real number
         */
        explicit Matrix4(float n);

        /**
         * Constructor from specific values for each row
         * @param a Vector4 object 1rt row
         * @param b Vector4 object 2nd row
         * @param c Vector4 object 3rd row
         * @param d Vector4 object 4th row
         */
        Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);

        /**
         * @brief Constructor from C-Style array
         * @param m C-Style array of Vector4
         */
        explicit Matrix4(const Vector4 m[4]);

        /**
         * @brief Copy constructor
         * @param m Matrix4 object
         */
        Matrix4(const Matrix4& m);

        /**
         * @brief Copy operator
         * @param m Matrix4 object
         * @return Vector4 reference
         */
        Matrix4& operator=(const Matrix4& m);

        /**
         * @brief Random access operator
         * @param row Row access index
         * @return Reference of row
         */
        Vector4& operator[](size_t row);

        /**
         * @brief Random access operator
         * @param row Row access index
         * @return Const reference of row
         */
        const Vector4& operator[](size_t row) const;

        /**
         * @brief Matrix addition operator
         * @param m Matrix4 object
         * @return Copy of this matrix summed by other matrix
         */
        Matrix4 operator+(const Matrix4& m) const;

        /**
         * @brief Matrix subtraction operator
         * @param m Matrix4 object
         * @return Copy of this matrix subtracted by other matrix
         */
        Matrix4 operator-(const Matrix4& m) const;

        /**
         * @brief Scalar multiplication operator
         * @param n Scalar
         * @return Copy of this matrix scaled by real number
         */
        Matrix4 operator*(float n) const;

        /**
         * @brief Matrix-Vector multiplication operator
         * @param v Vector4 object
         * @return Vector result of multiplication
         */
        Vector4 operator*(const Vector4& v) const;

        /**
         * @brief Matrix multiplication operator
         * @param m Matrix4 object
         * @return Matrix multiplication result
         */
        Matrix4 operator*(const Matrix4& m) const;

        /**
         * @brief Matrix division operations
         * @param n Scalar
         * @return Copy of matrix scaled by 1 / n
         */
        Matrix4 operator/(float n) const;

        /**
         * @brief Matrix addition and assigment operator
         * @param m Matrix4 object
         * @return This matrix reference
         */
        Matrix4& operator+=(const Matrix4& m);

        /**
         * @brief Matrix subtraction and assigment operator
         * @param m Matrix4 object
         * @return This matrix reference
         */
        Matrix4& operator-=(const Matrix4& m);

        /**
         * @brief Matrix multiplication and assigment operator
         * @param n Scalar
         * @return This matrix reference
         */
        Matrix4& operator*=(float n);

        /**
         * @brief Matrix multiplication and assigment operator
         * @param m Matrix4 object
         * @return This matrix reference
         */
        Matrix4& operator*=(const Matrix4& m);

        /**
         * @brief Vector division and assigment operator
         * @param n Scalar
         * @return This matrix reference
         */
        Matrix4& operator/=(float n);

        /**
         * @brief Matrix comparison bool
         * @param m Matrix4 object
         * @return The two matrices are equal
         */
        bool operator==(const Matrix4& m) const;

        /**
         * @brief Matrix comparison bool
         * @param m Matrix3 object
         * @return The two matrices are not equal
         */
        bool operator!=(const Matrix4& m) const;

        /**
         *
         * @return Determinant of matrix
         */
        [[nodiscard]] float determinant() const;

        /**
         *
         * @return Adjugate matrix
         */
        [[nodiscard]] Matrix4 adjugate() const;

        /**
         *
         * @return Matrix inverse
         */
        [[nodiscard]] Matrix4 inverse() const;

        /**
         *
         * @return Matrix transpose
         */
        [[nodiscard]] Matrix4 transpose() const;

        /**
         *
         * @param newLine Define if each row was separated by new line instead space. By default is true
         * @return Matrix string
         */
        [[nodiscard]] std::string str(bool newLine = true) const;

    private:
        Vector4 rows[4];
    };

    inline Matrix4 operator*(float n, const Matrix4& m) {
        return m * n;
    }

    inline Matrix4 operator/(float n, const Matrix4& m) {
        return m / n;
    }

    using mat4 = Matrix4;


} // namespace re


#endif //RAVENENGINE_MATRIX4_HPP
