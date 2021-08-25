#ifndef RAVENENGINE_TRANSFORM_HPP
#define RAVENENGINE_TRANSFORM_HPP


#include "Component.hpp"

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"


namespace re {

    class Transform : public  Component {
    public:
        Transform(const vec3& position, const vec3& scale, const vec3& angles);

        Transform(const vec3& position, const vec3& scale, const vec3& angles, Entity* owner);

        [[nodiscard]] Matrix4 getWorldMatrix() const;

    public:
        Vector3 position;
        Vector3 scale;
        Vector3 eulerAngles;
        Quaternion rotation;
    };

} // namespace re


#endif //RAVENENGINE_TRANSFORM_HPP
