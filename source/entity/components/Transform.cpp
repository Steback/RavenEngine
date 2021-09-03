#include "Transform.hpp"

#include "math/Matrix3.hpp"


namespace re {

    Transform::Transform(const vec3 &position, const vec3 &scale, const vec3 &angles)
            : Component(nullptr), position(position), scale(scale) {
        rotation = Quaternion(angles);
    }

    Transform::Transform(const vec3 &position, const vec3 &scale, const quat &rotation)
            : Component(nullptr), position(position), scale(scale), rotation(rotation) {

    }

    Transform::Transform(const vec3 &position, const vec3 &scale, const vec3 &angles, Entity *owner)
            : Component(owner), position(position), scale(scale) {
        rotation = Quaternion(angles);
    }

    Transform::Transform(const vec3 &position, const vec3 &scale, const quat &rotation, Entity *owner)
            : Component(owner), position(position), scale(scale), rotation(rotation) {

    }

    Transform::Transform(json &component, Entity *owner) : Component(owner) {
        serialize(component);
    }

    Matrix4 Transform::getWorldMatrix() const {
        Matrix3 rotationMatrix = rotation.unit().getRotationMatrix();

        return {
                { scale.x * rotationMatrix[0][0], scale.x * rotationMatrix[1][0], scale.x * rotationMatrix[2][0], 0.0f },
                { scale.y * rotationMatrix[0][1], scale.y * rotationMatrix[1][1], scale.y * rotationMatrix[2][1], 0.0f },
                { scale.z * rotationMatrix[0][2], scale.z * rotationMatrix[1][2], scale.z * rotationMatrix[2][2], 0.0f },
                { position.x, position.y, position.z, 1.0f }
        };
    }

    json Transform::serialize() {
        return {
            {"position", {
                {"x", position.x},
                {"y", position.y},
                {"z", position.z}
            }},
            {"rotation", {
                {"w", rotation.w},
                {"x", rotation.x},
                {"y", rotation.y},
                {"z", rotation.x}
            }},
            {"scale", {
                {"x", scale.x},
                {"y", scale.y},
                {"z", scale.z}
            }}
        };
    }

    void Transform::serialize(json &component) {
        auto pos = component["position"];
        position = vec3(pos["x"].get<float>(), pos["y"].get<float>(), pos["z"].get<float>());

        auto rot = component["rotation"];
        rotation = quat(rot["w"].get<float>(), rot["x"].get<float>(), rot["y"].get<float>(), rot["z"].get<float>());

        auto sle = component["scale"];
        scale = vec3(sle["x"].get<float>(), sle["y"].get<float>(), sle["z"].get<float>());
    }

    Vector3 Transform::getEulerAngles() const {
        return rotation.getEulerAngles();
    }

    void Transform::setEulerAngles(const vec3 &angles) {
        rotation = Quaternion(angles);
    }

} // namespace re
