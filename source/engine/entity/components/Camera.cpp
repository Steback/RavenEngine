#include "Camera.hpp"

#include <cmath>

#include "nameof.hpp"

#include "Transform.hpp"
#include "engine/math/Basis.hpp"
#include "engine/entity/Entity.hpp"


namespace re {

    /**
     *
     * @param fov Field of View(FOV) angle in radians
     * @param zNear Near plane distance
     * @param zFar Far plane distance
     * @param target Camera target
     * @param owner Valid Entity pointer
     */
    Camera::Camera(float fov, float zNear, float zFar, const Vector3& target, Entity* owner)
            : Component(owner), fov(fov), zNear(zNear), zFar(zFar), target(target) {

    }

    Camera::Camera(json &component, Entity *owner) : Component(owner) {
        serialize(component);
    }

    /**
     * @brief Set projection as a Orthographic
     * @param left Left plane distance
     * @param right Right plane distance
     * @param top Top plane distance
     * @param bottom Bottom plane distance
     */
    void Camera::setOrthographic(float left, float right, float top, float bottom) {
        projection = mat4(1.0f);
        projection[0][0] = 2.0f / (right - left);
        projection[1][1] = 2.0f / (bottom - top);
        projection[2][2] = 1.0f / (zFar - zNear);
        projection[3][0] = -(right + left) / (right - left);
        projection[3][1] = -(bottom + top) / (bottom - top);
        projection[3][2] = -zNear / (zFar - zNear);
    }

    /**
     * @brief Set projection as a Perspective
     * @param aspect Aspect ratio of current viewport
     */
    void Camera::setPerspective(float aspect) {
        const float tanHalfFovy = std::tan(fov * 0.5f);
        projection = mat4{0.0f};
        projection[0][0] = 1.0f / (aspect * tanHalfFovy);
        projection[1][1] = 1.0f / (tanHalfFovy);
        projection[2][2] = zFar / (zFar - zNear);
        projection[2][3] = 1.0f;
        projection[3][2] = -(zFar * zNear) / (zFar - zNear);

    }

    /**
     * @brief Set view to look at a target
     * @param up [Optional] Up world vector, By default is (0, 1, 0).
     * @note The function not require the eye and center as a parameters. Eye vector is the target attribute of Camera
     * component, and Center is position attribute in Transform component
     */
    void Camera::lookAt(const Vector3& up) {
        vec3 position = owner->getComponent<Transform>().position;
        const vec3 f((target - position).normalized());
        const vec3 s(f.cross(up).normalized());
        const vec3 u(s.cross(f));

        view.setIdentity();
        view[0][0] = s.x;
        view[1][0] = s.y;
        view[2][0] = s.z;
        view[0][1] = u.x;
        view[1][1] = -u.y;
        view[2][1] = u.z;
        view[0][2] = f.x;
        view[1][2] = f.y;
        view[2][2] = f.z;
        view[3][0] = -s.dot(position);
        view[3][1] = -u.dot(position);
        view[3][2] = -f.dot(position);
    }

    void Camera::update() {
        lookAt();
    }

    json Camera::serialize() {
        return {
            {std::string(NAMEOF(fov)), fov},
            {std::string(NAMEOF(zNear)), zNear},
            {std::string(NAMEOF(zFar)), zFar},
            {std::string(NAMEOF(target)), target.values},
        };
    }

    void Camera::serialize(json &component) {
        fov = component[std::string(NAMEOF(fov))].get<float>();
        zNear = component[std::string(NAMEOF(zNear))].get<float>();
        zFar = component[std::string(NAMEOF(zFar))].get<float>();
        target = Vector3(component[std::string(NAMEOF(target))].get<std::array<float, 3>>().data());
    }

} // namespace re
