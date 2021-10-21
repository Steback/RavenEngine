#include "Transform.hpp"

#include "nameof.hpp"

#include "engine/math/Basis.hpp"


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
        Basis rotationMatrix = Basis(rotation, scale);
        return {
            { rotationMatrix[0][0], rotationMatrix[1][0], rotationMatrix[2][0], 0.0f },
            { rotationMatrix[0][1], rotationMatrix[1][1], rotationMatrix[2][1], 0.0f },
            { rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2], 0.0f },
            { position.x, position.y, position.z, 1.0f }
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
        return rotation.getAngles();
    }

    /**
     *
     * @param angles Set Quaternion form Euler Angles
     */
    void Transform::setEulerAngles(const vec3 &angles) {
        rotation = Quaternion(angles);
    }

} // namespace re
