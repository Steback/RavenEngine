#ifndef RAVENENGINE_BASIS_HPP
#define RAVENENGINE_BASIS_HPP


#include "Vector3.hpp"
#include "Quaternion.hpp"


namespace re {

    class Basis {
    public:
        inline Basis() = default;

        inline explicit Basis(float n);

        inline Basis(const Vector3& x, const Vector3& y, const Vector3& z);

        inline explicit Basis(const Quaternion& q, const Vector3& s = {1.0f, 1.0f, 1.0f});

        inline Basis(float angle, const Vector3& axis, const Vector3& s = {1.0f, 1.0f, 1.0f});

        inline explicit Basis(const Vector3& angles, const Vector3& s = {1.0f, 1.0f, 1.0f});

        inline Basis(const Basis&) = default;

        void invert();

        Basis inverted();

        inline void transpose();

        inline Basis transposed();

        inline float determinant();

        void rotate(const Quaternion& q, const Vector3& s = {1.0f, 1.0f, 1.0f});

        [[nodiscard]] Basis rotated(const Quaternion& q, const Vector3& s = {1.0f, 1.0f, 1.0f}) const;

        void rotate(float angle, const Vector3& axis, const Vector3& s = {1.0f, 1.0f, 1.0f});

        [[nodiscard]] Basis rotated(float angle, const Vector3& axis, const Vector3& s = {1.0f, 1.0f, 1.0f}) const;

        void rotate(const Vector3& angles, const Vector3& s = {1.0f, 1.0f, 1.0f});

        [[nodiscard]] Basis rotated(const Vector3& angles, const Vector3& s = {1.0f, 1.0f, 1.0f}) const;

        inline void scale(const Vector3& s);

        inline Basis scaled(const Vector3& s);

        void setQuaternion(const Quaternion& q, const Vector3& s = {1.0f, 1.0f, 1.0f});

        [[nodiscard]] Quaternion getQuaternion() const;

        inline void setEuler(const Vector3& angles, const Vector3& s = {1.0f, 1.0f, 1.0f});

        [[nodiscard]] inline Vector3 getEuler() const;

        void setAxis(float angle, const Vector3& axis, const Vector3& s = {1.0f, 1.0f, 1.0f});

        void getAxis(float& angle, Vector3& axis);

        inline void setIdentity();

        inline void setZero();

        inline void setDiagonal(const Vector3& dig);

        [[nodiscard]] std::string str() const;

        static Basis eulerAngles2MatrixXZY(const Vector3& angles);

        static Basis eulerAngles2MatrixXYZ(const Vector3& angles);

        static Basis eulerAngles2MatrixYXZ(const Vector3& angles);

        static Basis eulerAngles2MatrixYZX(const Vector3& angles);

        static Basis eulerAngles2MatrixZXY(const Vector3& angles);

        static Basis eulerAngles2MatrixZYX(const Vector3& angles);

        static Vector3 matrix2EulerAnglesXZY(const Basis& dcm);

        static Vector3 matrix2EulerAnglesXYZ(const Basis& dcm);

        static Vector3 matrix2EulerAnglesYZX(const Basis& dcm);

        static Vector3 matrix2EulerAnglesYXZ(const Basis& dcm);

        static Vector3 matrix2EulerAnglesZXY(const Basis& dcm);

        static Vector3 matrix2EulerAnglesZYX(const Basis& dcm);

        static Basis rotationX(float theta);

        static Basis rotationY(float theta);

        static Basis rotationZ(float theta);

        inline Basis& operator=(const Basis&) = default;

        inline Basis& operator=(Basis&&) = default;

        inline Vector3& operator[](size_t i);

        inline const Vector3& operator[](size_t i) const;

        inline Basis operator-() const;

        inline void operator+=(const Basis& m);

        inline void operator-=(const Basis& m);

        inline void operator*=(float n);

        inline void operator*=(const Basis& m);

        inline void operator/=(float n);

        inline Basis operator+(const Basis& m) const;

        inline Basis operator-(const Basis& m) const;

        inline Basis operator*(float n) const;

        inline Vector3 operator*(const Vector3& v) const;

        Basis operator*(const Basis& m) const;

        inline Basis operator/(float n) const;

        inline bool operator==(const Basis& m) const;

        inline bool operator!=(const Basis& m) const;

        Vector3 elements[3] = {
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
        };
    };

    Basis::Basis(float n) {
        elements[0][0] = n;
        elements[0][1] = 0.0f;
        elements[0][2] = 0.0f;
        elements[1][0] = 0.0f;
        elements[1][2] = n;
        elements[1][3] = 0.0f;
        elements[2][0] = 0.0f;
        elements[2][1] = 0.0f;
        elements[2][2] = n;
    }

    Basis::Basis(const Vector3 &x, const Vector3 &y, const Vector3 &z) {
        elements[0] = x;
        elements[1] = y;
        elements[2] = z;
    }

    Basis::Basis(const Quaternion &q, const Vector3 &s) {
        setQuaternion(q, s);
    }

    Basis::Basis(float angle, const Vector3 &axis, const Vector3 &s) {
        setAxis(angle, axis, s);
    }

    Basis::Basis(const Vector3 &angles, const Vector3 &s) {
        setEuler(angles, s);
    }

    void Basis::transpose() {
        *this = transposed();
    }

    Basis Basis::transposed() {
        return {
            {elements[0][0], elements[1][0], elements[2][0]},
            {elements[0][1], elements[1][1], elements[2][1]},
            {elements[0][2], elements[1][2], elements[2][2]}
        };
    }

    float Basis::determinant() {
        return (elements[0][0] * elements[1][1] * elements[2][2]) +
               (elements[0][1] * elements[1][2] * elements[2][0]) +
               (elements[0][2] * elements[1][0] * elements[2][1]) -
               (elements[0][2] * elements[1][1] * elements[2][0]) -
               (elements[0][0] * elements[1][2] * elements[2][1]) -
               (elements[0][1] * elements[1][0] * elements[2][2]);
    }

    void Basis::scale(const Vector3 &s) {
        *this = scaled(s);
    }

    Basis Basis::scaled(const Vector3 &s) {
        return {
            elements[0] * s.x,
            elements[1] * s.y,
            elements[2] * s.z
        };
    }

    void Basis::setEuler(const Vector3 &angles, const Vector3 &s) {
        *this = eulerAngles2MatrixYXZ(angles);
        scale(s);
    }

    Vector3 Basis::getEuler() const {
        return matrix2EulerAnglesYXZ(*this);
    }

    void Basis::setIdentity() {
        elements[0][0] = 1.0f;
        elements[0][1] = 0.0f;
        elements[0][2] = 0.0f;
        elements[1][0] = 0.0f;
        elements[1][1] = 1.0f;
        elements[1][2] = 0.0f;
        elements[2][0] = 0.0f;
        elements[2][1] = 0.0f;
        elements[2][2] = 1.0f;
    }

    void Basis::setZero() {
        elements[0][0] = 0.0f;
        elements[0][1] = 0.0f;
        elements[0][2] = 0.0f;
        elements[1][0] = 0.0f;
        elements[1][1] = 0.0f;
        elements[1][2] = 0.0f;
        elements[2][0] = 0.0f;
        elements[2][1] = 0.0f;
        elements[2][2] = 0.0f;
    }

    void Basis::setDiagonal(const Vector3 &dig) {
        elements[0][0] = dig.x;
        elements[0][1] = 0.0f;
        elements[0][2] = 0.0f;
        elements[1][0] = 0.0f;
        elements[1][1] = dig.y;
        elements[1][2] = 0.0f;
        elements[2][0] = 0.0f;
        elements[2][1] = 0.0f;
        elements[2][2] = dig.z;
    }

    Vector3 &Basis::operator[](size_t i) {
        return elements[i];
    }

    const Vector3 &Basis::operator[](size_t i) const {
        return elements[i];
    }

    Basis Basis::operator-() const {
        return {-elements[0], -elements[1], -elements[2]};
    }

    void Basis::operator+=(const Basis &m) {
        *this = *this + m;
    }

    void Basis::operator-=(const Basis &m) {
        *this = *this - m;
    }

    void Basis::operator*=(float n) {
        *this = *this * n;
    }

    void Basis::operator*=(const Basis &m) {
        *this = *this * m;
    }

    void Basis::operator/=(float n) {
        *this = *this / n;
    }

    Basis Basis::operator+(const Basis &m) const {
        return {elements[0] + m[0], elements[1] + m[1], elements[2] + m[2]};
    }

    Basis Basis::operator-(const Basis &m) const {
        return {elements[0] - m[0], elements[1] - m[1], elements[2] - m[2]};
    }

    Basis Basis::operator*(float n) const {
        return {elements[0] * n, elements[1] * n, elements[2] * n};
    }

    Vector3 Basis::operator*(const Vector3 &v) const {
        return {elements[0].dot(v), elements[1].dot(v), elements[2].dot(v)};
    }

    Basis Basis::operator/(float n) const {
        return {elements[0] / n, elements[1] / n, elements[2] / n};
    }

    inline Basis operator*(float s, const Basis& m) {
        return m * s;
    }

    inline Basis operator/(float s, const Basis& m) {
        return m / s;
    }

    bool Basis::operator==(const Basis &m) const {
        return elements[0] == m[0] && elements[1] == m[1] && elements[2] == m[2];
    }

    bool Basis::operator!=(const Basis &m) const {
        return elements[0] != m[0] && elements[1] != m[1] && elements[2] != m[2];
    }

} // namespace re

#endif //RAVENENGINE_BASIS_HPP
