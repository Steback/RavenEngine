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
         * @param rotation
         */
        Transform(const vec3& position, const vec3& scale, const quat& rotation);

        /**
         *
         * @param position
         * @param scale
         * @param angles Euler angles in radians
         * @param owner Valid pointer to entity that owns this component
         */
        Transform(const vec3& position, const vec3& scale, const vec3& angles, Entity* owner);

        /**
         *
         * @param position
         * @param scale
         * @param rotation
         * @param owner Valid pointer to entity that owns this component
         */
        Transform(const vec3& position, const vec3& scale, const quat& rotation, Entity* owner);

        /**
         * @brief Construct entity from JSON
         * @param component json object
         * @param owner Valid pointer to entity that owns this component
         */
        Transform(json& component, Entity* owner);

        [[nodiscard]] Matrix4 getWorldMatrix() const;

        /**
         * @brief Serialize Transform to JSON
         */
        json serialize() override;

        /**
         * @brief Serialize Transform from JSON
         */
        void serialize(json &component) override;

        /**
         *
         * @return Euler angles of Quaternion
         */
        [[nodiscard]] Vector3 getEulerAngles() const;

        /**
         * Set Quaternion by Euler Angles
         * @param angles Euler angles in radians
         */
        void setEulerAngles(const vec3& angles);

    public:
        Vector3 position;
        Vector3 scale;
        Quaternion rotation;
    };

} // namespace re


#endif //RAVENENGINE_TRANSFORM_HPP
