#ifndef RAVENENGINE_CAMERA_HPP
#define RAVENENGINE_CAMERA_HPP


#include "math/Vector3.hpp"
#include "math/Matrix4.hpp"


namespace re {

    /**
     * @brief Camera render class
     */
    class Camera {
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

        /**
         * @brief Define projection matrix as Perspective
         * @param fovy
         * @param aspect
         * @param near
         * @param far
         */
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        /**
         * @brief Set View to a specific direction
         * @param position
         * @param direction
         * @param up By default is {0, -1, 0}
         */
        void setViewDirection(const vec3& position, const vec3& direction, const vec3& up = {0.0f, -1.0f, 0.0f});

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
