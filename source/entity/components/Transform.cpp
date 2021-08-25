#include "Transform.hpp"

#include "math/Matrix3.hpp"


namespace re {

    Transform::Transform(const vec3 &position, const vec3 &scale, const vec3 &angles)
            : Component(nullptr), position(position), scale(scale), eulerAngles(angles) {
        rotation = Quaternion(eulerAngles);
    }

    Transform::Transform(const vec3 &position, const vec3 &scale, const vec3 &angles, Entity *owner)
            : Component(owner), position(position), scale(scale), eulerAngles(angles) {
        rotation = Quaternion(eulerAngles);
    }

    Matrix4 Transform::getWorldMatrix() const {
        Matrix3 rotationMatrix = rotation.getRotationMatrix();

        return {
                { scale.x * rotationMatrix[0][0], scale.x * rotationMatrix[1][0], scale.x * rotationMatrix[2][0], 0.0f },
                { scale.y * rotationMatrix[0][1], scale.y * rotationMatrix[1][1], scale.y * rotationMatrix[2][1], 0.0f },
                { scale.z * rotationMatrix[0][2], scale.z * rotationMatrix[1][2], scale.z * rotationMatrix[2][2], 0.0f },
                { position.x, position.y, position.z, 1.0f }
        };
    }

} // namespace re
