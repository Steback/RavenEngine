#include "Camera.hpp"

#include <cmath>

#include "math/Matrix3.hpp"


namespace re {

    Camera::Camera() = default;

    void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
        projection = mat4(1.0f);
        projection[0][0] = 2.0f / (right - left);
        projection[1][1] = 2.0f / (bottom - top);
        projection[2][2] = 1.0f / (far - near);
        projection[3][0] = -(right + left) / (right - left);
        projection[3][1] = -(bottom + top) / (bottom - top);
        projection[3][2] = -near / (far - near);
    }

    void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
        const float tanHalfFovy = std::tan(fovy * 0.5f);
        projection = mat4{0.0f};
        projection[0][0] = 1.0f / (aspect * tanHalfFovy);
        projection[1][1] = 1.0f / (tanHalfFovy);
        projection[2][2] = far / (far - near);
        projection[2][3] = 1.0f;
        projection[3][2] = -(far * near) / (far - near);

    }

    // TODO: Check for use Quaternions in View Matrix
    void Camera::setViewDirection(const vec3 &position, const vec3 &rotation) {
        const float c3 = std::cos(rotation.z);
        const float s3 = std::sin(rotation.z);
        const float c2 = std::cos(rotation.x);
        const float s2 = std::sin(rotation.x);
        const float c1 = std::cos(rotation.y);
        const float s1 = std::sin(rotation.y);
        const vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
        const vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
        const vec3 w{(c2 * s1), (-s2), (c1 * c2)};
        view = mat4{1.f};
        view[0][0] = u.x;
        view[1][0] = u.y;
        view[2][0] = u.z;
        view[0][1] = v.x;
        view[1][1] = v.y;
        view[2][1] = v.z;
        view[0][2] = w.x;
        view[1][2] = w.y;
        view[2][2] = w.z;
        view[3][0] = -(u * position);
        view[3][1] = -(v * position);
        view[3][2] = -(w * position);
    }

    // TODO: Implement Camera to look at
    void Camera::setViewTarget(const vec3 &position, const vec3 &target, const vec3 &up) {
//        setViewDirection(position, target - position, up);
    }

    const Matrix4 &Camera::getProjection() const {
        return projection;
    }

    const Matrix4 &Camera::getView() const {
        return view;
    }

} // namespace re
