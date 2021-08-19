#ifndef RAVENENGINE_QUATERNION_HPP
#define RAVENENGINE_QUATERNION_HPP


#include <limits>
#include <string>


namespace re {

    class Vector3;
    class Matrix3;

    /**
     * @brief Quaternion class
     *
     * W X Y Z\n
     * The W component is first
     */
    class Quaternion {
    public:
        /**
         * @brief Default Constructor
         */
        Quaternion();

        /**
         * @brief Constructor for set real and vector part of Quaternion
         * @param s Real part
         * @param i Vector part 1
         * @param j Vector part 2
         * @param k Vector part 3
         */
        Quaternion(float s, float i, float j, float k);

        /**
         * @brief Constructor for set real and vector part of Quaternion
         * @param s Real part
         * @param v Vector part
         */
        Quaternion(float s, const Vector3& v);

        /**
         * @brief Construct Quaternion from Euler angles
         * @param eulerAngles Vector 3 with yaw (Z), pitch (Y), roll (X)
         *
         * This conversion use the sequence Z-Y-X.
         * <a href="https://www.wikiwand.com/en/Conversion_between_quaternions_and_Euler_angles#/Euler_angles_to_quaternion_conversion"> Explain </a>
         */
        explicit Quaternion(const Vector3& eulerAngles);

        bool operator==(const Quaternion& q) const;

        bool operator!=(const Quaternion& q) const;

        Quaternion operator-() const;

        /**
         * @brief Quaternion addition
         * @param n Scalar
         * @return Result of addition
         */
        Quaternion operator+(float n) const;

        /**
         * @brief Quaternion addition
         * @param q Quaternion
         * @return Result of addition
         */
        Quaternion operator+(const Quaternion& q) const;

        /**
         * @brief Quaternion subtraction
         * @param n Scalar
         * @return Result of subtraction
         */
        Quaternion operator-(float n) const;

        /**
         * @brief Quaternion subtraction
         * @param q Quaternion
         * @return Result of subtraction
         */
        Quaternion operator-(const Quaternion& q) const;

        /**
         * @brief Quaternion multiplication
         * @param n Scalar
         * @return Result of multiplication
         */
        Quaternion operator*(float n) const;

        /**
         * @brief Quaternion multiplication
         * @param q Quaternion object
         * @return Result of multiplication
         */
        Quaternion operator*(const Quaternion& q) const;

        /**
         * @brief Quaternions division
         * @param n Scalar
         * @return Quaternion * (1 / n)
         */
        Quaternion operator/(float n) const;

        /**
         * @brief Quaternion addition and assigment
         * @param n Scalar
         * @return Referee of Quaternions
         */
        Quaternion& operator+=(float n);

        /**
         * @brief Quaternion addition and assigment
         * @param q Quaternion
         * @return Referee of Quaternions
         */
        Quaternion& operator+=(const Quaternion& q);

        /**
         * @brief Quaternion subtraction and assigment
         * @param n Scalar
         * @return Referee of Quaternions
         */
        Quaternion& operator-=(float n);

        /**
         * @brief Quaternion subtraction and assigment
         * @param q Quaternion
         * @return Referee of Quaternions
         */
        Quaternion& operator-=(const Quaternion& q);

        /**
         * @brief Quaternion multiplication and assigment
         * @param n Scalar
         * @return Referee of Quaternions
         */
        Quaternion& operator*=(float n);

        /**
         * @brief Quaternion multiplication and assigment
         * @param q Quaternion
         * @return Referee of Quaternions
         */
        Quaternion& operator*=(const Quaternion& q);

        /**
         * @brief Quaternion division and assigment
         * @param n Scalar
         * @return Referee of Quaternions
         */
        Quaternion& operator/=(float n);

        /**
         *
         * @return Vector part of Quaternion
         */
        [[nodiscard]] Vector3 getVectorPart() const;

        /**
         *
         * @return Quaternion as a rotation matrix
         */
        [[nodiscard]] Matrix3 getRotationMatrix() const;

        /**
         * @brief Set Quaternion from a rotation matrix
         * @param m Rotation matrix
         */
        void setRotationMatrix(const Matrix3& m);

        /**
         * @brief Get Euler angles from Quaternion.
         * <a href="https://www.wikiwand.com/en/Conversion_between_quaternions_and_Euler_angles#/Quaternion_to_Euler_angles_conversion"> Explain </a>
         * @return Vector3 with the yaw (Z), pitch (Y), roll (X) angles
         */
        [[nodiscard]] Vector3 getEulerAngles() const;

        /**
         *
         * @return Conjugate of Quaternion
         */
        [[nodiscard]] Quaternion conjugate() const;

        /**
         *
         * @return Magnitude of Quaternion
         */
        [[nodiscard]] float norm() const;

        /**
         * Convert Quaternion into a unit-quaternion
         */
        void normalise();

        /**
         *
         * @return Inverse of Quaternions
         */
        [[nodiscard]] Quaternion inverse() const;

        /**
         *
         * @return Unit-Quaternion of this Quaternion
         */
        [[nodiscard]] Quaternion unit() const;

        /**
         *
         * @param q Quaternion
         * @return Dot product of this quaternions and q
         */
        [[nodiscard]] float dot(const Quaternion& q) const;

        /**
         *
         * @return C-Style array of this Quaternion
         */
        float* ptr();

        /**
         *
         * @return String of this Quaternion
         */
        [[nodiscard]] std::string str() const;

    public:
        float w{}, x{}, y{}, z{};
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
