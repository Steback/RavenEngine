#include "Camera.hpp"

#include <cmath>

#include "nameof.hpp"

#include "Transform.hpp"
#include "math/Matrix3.hpp"
#include "entity/Entity.hpp"


namespace re {

    Camera::Camera(Type type, float fov, float zNear, float zFar, Entity* owner)
            : Component(owner), type(type), fov(fov), zNear(zNear), zFar(zFar) {

    }

    Camera::Camera(json &component, Entity *owner) : Component(owner) {
        serialize(component);
    }

    void Camera::setOrthographicProjection(float left, float right, float top, float bottom) {
        projection = mat4(1.0f);
        projection[0][0] = 2.0f / (right - left);
        projection[1][1] = 2.0f / (bottom - top);
        projection[2][2] = 1.0f / (zFar - zNear);
        projection[3][0] = -(right + left) / (right - left);
        projection[3][1] = -(bottom + top) / (bottom - top);
        projection[3][2] = -zNear / (zFar - zNear);
    }

    void Camera::setPerspectiveProjection(float aspect) {
        const float tanHalfFovy = std::tan(fov * 0.5f);
        projection = mat4{0.0f};
        projection[0][0] = 1.0f / (aspect * tanHalfFovy);
        projection[1][1] = 1.0f / (tanHalfFovy);
        projection[2][2] = zFar / (zFar - zNear);
        projection[2][3] = 1.0f;
        projection[3][2] = -(zFar * zNear) / (zFar - zNear);

    }

    void Camera::setViewDirection(const vec3 &position, const quat &rotation) {
        mat3 rotMatrix = rotation.rotationMatrix();
        const vec3 u{rotMatrix[0]};
        const vec3 v{rotMatrix[1]};
        const vec3 w{rotMatrix[2]};

        view = mat4{1.f};
        view[0][0] = u.x;
        view[1][0] = u.y;
        view[2][0] = u.z;
        view[0][1] = v.x;
        view[1][1] = -v.y;
        view[2][1] = v.z;
        view[0][2] = w.x;
        view[1][2] = w.y;
        view[2][2] = w.z;
        view[3][0] = -(u.dot(position));
        view[3][1] = -(v.dot(position));
        view[3][2] = -(w.dot(position));
    }

    // TODO: Implement look at Camera
    void Camera::setViewTarget(const vec3 &position, const vec3 &target, const vec3 &up) {
//        setViewDirection(position, target - position, up);
    }

    void Camera::update() {
        auto& transform = owner->getComponent<Transform>();
        switch (type) {
            case Camera::DIRECTION:
                setViewDirection(transform.position, transform.rotation);
                break;
            case Camera::LOOK_AT:
                // TODO: Implement look at Camera
                break;
        }
    }

    const Matrix4 &Camera::getProjection() const {
        return projection;
    }

    const Matrix4 &Camera::getView() const {
        return view;
    }

    json Camera::serialize() {
        return {
            {std::string(NAMEOF(type)), type},
            {std::string(NAMEOF(fov)), fov},
            {std::string(NAMEOF(zNear)), zNear},
            {std::string(NAMEOF(zFar)), zFar},
        };
    }

    void Camera::serialize(json &component) {
        type = component[std::string(NAMEOF(type))].get<Type>();
        fov = component[std::string(NAMEOF(fov))].get<float>();
        zNear = component[std::string(NAMEOF(zNear))].get<float>();
        zFar = component[std::string(NAMEOF(zFar))].get<float>();
    }

} // namespace re
