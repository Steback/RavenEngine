#ifndef RAVENENGINE_CAMERA_HPP
#define RAVENENGINE_CAMERA_HPP


#include "math/Vector3.hpp"
#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"


namespace re {

    /**
     * @brief Camera render class
     */
    class Camera {
    public:
        /**
         * @brief Camera Type
         */
        enum Type {
            DIRECTION = 1,
            LOOK_AT = 2
        };

    public:
        Camera();

        /**
         * @brief Define projection matrix as Orthographic
         * @param left
         * @param right
         * @param top
         * @param bottom
         * @param near
         * @param far
         */
        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        // TODO: Change camera direction to use Quaternions
        /**
         * @brief Define projection matrix as Perspective
         * @param fovy
         * @param aspect
         * @param near
         * @param far
         */
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        /**
         * @brief Set View to a specific rotation
         * @param position
         * @param rotation Quaternion with orientation
         */
        void setViewDirection(const vec3 &position, const vec3 &rotation);

        /**
         * @brief Set view to look at a target
         * @param position
         * @param target
         * @param up By default is {0, -1, 0}
         */
        void setViewTarget(const vec3& position, const vec3& target, const vec3& up = {0.f, -1.f, 0.f});

        [[nodiscard]] const Matrix4& getProjection() const;

        [[nodiscard]] const Matrix4& getView() const;

    private:
        Matrix4 projection{1.0f};
        Matrix4 view{1.0f};
    };


} // namespace re


#endif //RAVENENGINE_CAMERA_HPP
