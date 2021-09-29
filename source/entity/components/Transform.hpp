#ifndef RAVENENGINE_TRANSFORM_HPP
#define RAVENENGINE_TRANSFORM_HPP


#include "Component.hpp"

#include "math/Matrix4.hpp"
#include "math/Matrix3.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"


namespace re {

    class Transform : public  Component {
    public:
        // TODO: Change this for a dynamic uniform object
        struct Ubo {
            mat4 mvp;
            mat4 invTransform;
        };

    public:
        Transform(const vec3& position, const vec3& scale, const vec3& angles, Entity* owner);

        Transform(const vec3& position, const vec3& scale, const quat& rotation, Entity* owner);

        Transform(json& component, Entity* owner);

        [[nodiscard]] Matrix4 worldMatrix() const;

        [[nodiscard]] Matrix3 normalMatrix() const;

        json serialize() override;

        void serialize(json &component) override;

        [[nodiscard]] Vector3 getEulerAngles() const;

        void setEulerAngles(const vec3& angles);

    public:
        Vector3 position;
        Vector3 scale;
        Quaternion rotation;
    };

} // namespace re


#endif //RAVENENGINE_TRANSFORM_HPP
