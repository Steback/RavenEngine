#ifndef RAVENENGINE_MATRIX3_HPP
#define RAVENENGINE_MATRIX3_HPP


#include "Vector3.hpp"


namespace re {

    class Matrix3 {
    public:
        /**
         * @brief Default Constructor
         */
        Matrix3();

        /**
         * Constructor for set each axis with the same value
         * @param n Real number
         */
        explicit Matrix3(float n);

        /**
         * Constructor from specific values for each row
         * @param a Vector3 object 1rt row
         * @param b Vector3 object 2nd row
         * @param c Vector3 object 3rd row
         */
        Matrix3(const Vector3& a, const Vector3& b, const Vector3& c);

        /**
         * @brief Constructor from C-Style array
         * @param m C-Style array of Vector3
         */
        explicit Matrix3(const Vector3 m[3]);

        /**
         * @brief Copy constructor
         * @param m Matrix3 object
         */
        Matrix3(const Matrix3& m);

        /**
         * @brief Copy operator
         * @param m Matrix3 object
         * @return Vector3 reference
         */
        Matrix3& operator=(const Matrix3& m);

        /**
         * @brief Random access operator
         * @param row Row access index
         * @return Reference of row
         */
        Vector3& operator[](size_t row);

        /**
         * @brief Random access operator
         * @param row Row access index
         * @return Const reference of row
         */
        const Vector3& operator[](size_t row) const;

        /**
         * @brief Matrix addition operator
         * @param m Matrix3 object
         * @return Copy of this matrix summed by other matrix
         */
        Matrix3 operator+(const Matrix3& m) const;

        /**
         * @brief Matrix subtraction operator
         * @param m Matrix3 object
         * @return Copy of this matrix subtracted by other matrix
         */
        Matrix3 operator-(const Matrix3& m) const;

        /**
         * @brief Scalar multiplication operator
         * @param n Scalar
         * @return Copy of this matrix scaled by real number
         */
        Matrix3 operator*(float n) const;

        /**
         * @brief Matrix-Vector multiplication operator
         * @param v Vector3 object
         * @return Vector result of multiplication
         */
        Vector3 operator*(const Vector3& v) const;

        /**
         * @brief Matrix multiplication operator
         * @param m Matrix3 object
         * @return Matrix multiplication result
         */
        Matrix3 operator*(const Matrix3& m) const;

        /**
         * @brief Matrix division operations
         * @param n Scalar
         * @return Copy of matrix scaled by 1 / n
         */
        Matrix3 operator/(float n) const;

        /**
         * @brief Matrix addition and assigment operator
         * @param m Matrix3 object
         * @return This matrix reference
         */
        Matrix3& operator+=(const Matrix3& m);

        /**
         * @brief Matrix subtraction and assigment operator
         * @param m Matrix3 object
         * @return This matrix reference
         */
        Matrix3& operator-=(const Matrix3& m);

        /**
         * @brief Matrix multiplication and assigment operator
         * @param n Scalar
         * @return This matrix reference
         */
        Matrix3& operator*=(float n);

        /**
         * @brief Matrix multiplication and assigment operator
         * @param m Matrix3 object
         * @return This matrix reference
         */
        Matrix3& operator*=(const Matrix3& m);

        /**
         * @brief Vector division and assigment operator
         * @param n Scalar
         * @return This matrix reference
         */
        Matrix3& operator/=(float n);

        /**
         * @brief Matrix comparison bool
         * @param m Matrix3 object
         * @return The two matrices are equal
         */
        bool operator==(const Matrix3& m) const;

        /**
         * @brief Matrix comparison bool
         * @param m Matrix3 object
         * @return The two matrices are not equal
         */
        bool operator!=(const Matrix3& m) const;

        /**
         *
         * @return Determinant of matrix
         */
        [[nodiscard]] float determinant() const;

        /**
         *
         * @return Adjugate matrix
         */
        [[nodiscard]] Matrix3 adjugate() const;

        /**
         *
         * @return Matrix inverse
         */
        [[nodiscard]] Matrix3 inverse() const;

        /**
         *
         * @return Matrix transpose
         */
        [[nodiscard]] Matrix3 transpose() const;

        /**
         *
         * @param newLine Define if each row was separated by new line instead space. By default is true
         * @return Matrix string
         */
        [[nodiscard]] std::string str(bool newLine = true) const;

    private:
        Vector3 rows[3];
    };

    inline Matrix3 operator*(float n, const Matrix3& m) {
        return m * n;
    }

    inline Matrix3 operator/(float n, const Matrix3& m) {
        return m * n;
    }

    using mat3 = Matrix3;


} // namespace re


#endif //RAVENENGINE_MATRIX3_HPP
