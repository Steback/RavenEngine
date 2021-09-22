#ifndef RAVENENGINE_QUATERNION_HPP
#define RAVENENGINE_QUATERNION_HPP


#include <limits>
#include <string>


namespace re {

    class Vector3;
    class Matrix3;

    class Quaternion {
    public:
        Quaternion();

        Quaternion(float s, float i, float j, float k);

        Quaternion(float s, const Vector3& v);

        explicit Quaternion(const Vector3& eulerAngles);

        explicit Quaternion(const float* p);

        explicit Quaternion(const double * p);

        Quaternion(const Quaternion& q);

        Quaternion& operator=(const Quaternion& q);

        float& operator[](size_t i);

        const float& operator[](size_t i) const;

        bool operator==(const Quaternion& q) const;

        bool operator!=(const Quaternion& q) const;

        Quaternion operator-() const;

        Quaternion operator+(float n) const;

        Quaternion operator+(const Quaternion& q) const;

        Quaternion operator-(float n) const;

        Quaternion operator-(const Quaternion& q) const;

        Quaternion operator*(float n) const;

        Quaternion operator*(const Quaternion& q) const;

        Quaternion operator/(float n) const;

        Quaternion& operator+=(float n);

        Quaternion& operator+=(const Quaternion& q);

        Quaternion& operator-=(float n);

        Quaternion& operator-=(const Quaternion& q);

        Quaternion& operator*=(float n);

        Quaternion& operator*=(const Quaternion& q);

        Quaternion& operator/=(float n);

        [[nodiscard]] Vector3 getVectorPart() const;

        [[nodiscard]] Matrix3 getRotationMatrix() const;

        void setRotationMatrix(const Matrix3& m);

        [[nodiscard]] Vector3 getEulerAngles() const;

        [[nodiscard]] Quaternion conjugate() const;

        [[nodiscard]] float norm() const;

        void normalise();

        [[nodiscard]] Quaternion inverse() const;

        [[nodiscard]] Quaternion unit() const;

        [[nodiscard]] float dot(const Quaternion& q) const;

        float* ptr();

        [[nodiscard]] std::string str() const;

        [[nodiscard]] float roll() const;

        [[nodiscard]] float pitch() const;

        [[nodiscard]] float yaw() const;

    public:
        float w{1.0f}, x{}, y{}, z{};
    };

    inline Quaternion operator+(float n, const Quaternion& q) {
        return q + n;
    }

    inline Quaternion operator-(float n, const Quaternion& q) {
        return q - n;
    }

    inline Quaternion operator*(float n, const Quaternion& q) {
        return q * n;
    }

    using quat = Quaternion;

} // namespace re


#endif //RAVENENGINE_QUATERNION_HPP
