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

        Matrix3& operator=(const Matrix3& m);

        Vector3& operator[](size_t row);

        const Vector3& operator[](size_t row) const;

        bool operator==(const Matrix3& m) const;

        bool operator!=(const Matrix3& m) const;

        Matrix3 operator-() const;

        Matrix3 operator+(const Matrix3& m) const;

        Matrix3 operator-(const Matrix3& m) const;

        Matrix3 operator*(float n) const;

        Vector3 operator*(const Vector3& v) const;

        Matrix3 operator*(const Matrix3& m) const;

        /**
         * @brief Matrix division operation
         * @param n Scalar
         * @return Copy of matrix scaled by 1 / n
         */
        Matrix3 operator/(float n) const;

        Matrix3& operator+=(const Matrix3& m);

        Matrix3& operator-=(const Matrix3& m);

        Matrix3& operator*=(float n);

        Matrix3& operator*=(const Matrix3& m);

        Matrix3& operator/=(float n);

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
