#include "Transform.hpp"

#include "nameof.hpp"


namespace re {

    /**
     *
     * @param position (X-Y-Z)
     * @param scale By default are 1-1-1
     * @param angles Euler angles
     * @param owner Valid pointer to Entity
     */
    Transform::Transform(const vec3 &position, const vec3 &scale, const vec3 &angles, Entity *owner)
            : Component(owner), position(position), scale(scale) {
        rotation = Quaternion(angles);
    }

    /**
     *
     * @param position (X-Y-Z)
     * @param scale By default are 1-1-1
     * @param rotation Quaternion with direction
     * @param owner Valid pointer to Entity
     */
    Transform::Transform(const vec3 &position, const vec3 &scale, const quat &rotation, Entity *owner)
            : Component(owner), position(position), scale(scale), rotation(rotation) {

    }

    Transform::Transform(json &component, Entity *owner) : Component(owner) {
        serialize(component);
    }

    /**
     *
     * @return Transform world matrix
     */
    Matrix4 Transform::worldMatrix() const {
        Matrix3 rotationMatrix = rotation.rotationMatrix();
        return {
            { scale.x * rotationMatrix[0][0], scale.x * rotationMatrix[1][0], scale.x * rotationMatrix[2][0], 0.0f },
            { scale.y * rotationMatrix[0][1], scale.y * rotationMatrix[1][1], scale.y * rotationMatrix[2][1], 0.0f },
            { scale.z * rotationMatrix[0][2], scale.z * rotationMatrix[1][2], scale.z * rotationMatrix[2][2], 0.0f },
            { position.x, position.y, position.z, 1.0f }
        };
    }

    // TODO: Fix normal transform matrix
    Matrix3 Transform::normalMatrix() const {
        Matrix3 rotationMatrix = rotation.norm() == 1.0f ? rotation.rotationMatrix() : rotation.unit().rotationMatrix();
        vec3 invScale = scale.inversed();
        return {
            { invScale.x * rotationMatrix[0][0], invScale.x * rotationMatrix[1][0], invScale.x * rotationMatrix[2][0] },
            { invScale.y * rotationMatrix[0][1], invScale.y * rotationMatrix[1][1], invScale.y * rotationMatrix[2][1] },
            { invScale.z * rotationMatrix[0][2], invScale.z * rotationMatrix[1][2], invScale.z * rotationMatrix[2][2] },
        };
    }

    json Transform::serialize() {
        return {
            {std::string(NAMEOF(position)), {
                position.x,
                position.y,
                position.z
            }},
            {std::string(NAMEOF(rotation)), {
                rotation.w,
                rotation.x,
                rotation.y,
                rotation.z
            }},
            {std::string(NAMEOF(scale)), {
                scale.x,
                scale.y,
                scale.z
            }}
        };
    }

    void Transform::serialize(json &component) {
        position = vec3(component[std::string(NAMEOF(position))].get<std::array<float, 3>>().data());
        rotation = quat(component[std::string(NAMEOF(rotation))].get<std::array<float, 4>>().data());
        scale = vec3(component[std::string(NAMEOF(scale))].get<std::array<float, 3>>().data());
    }

    /**
     *
     * @return Get Euler Angles from quaternion
     */
    Vector3 Transform::getEulerAngles() const {
        return rotation.getEulerAngles();
    }

    /**
     *
     * @param angles Set Quaternion form Euler Angles
     */
    void Transform::setEulerAngles(const vec3 &angles) {
        rotation = Quaternion(angles);
    }

} // namespace re
