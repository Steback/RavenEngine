#ifndef RAVENENGINE_TRANSFORM_HPP
#define RAVENENGINE_TRANSFORM_HPP


#include "Component.hpp"

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"


namespace re {

    /**
     * @brief Transform class
     */
    class Transform : public  Component {
    public:
        /**
         *
         * @param position
         * @param scale
         * @param angles Euler angles in radians
         */
        Transform(const vec3& position, const vec3& scale, const vec3& angles);

        /**
         *
         * @param position
         * @param scale
         * @param angles Euler angles in radians
         * @param owner Valid pointer to entity that owns this component
         */
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
