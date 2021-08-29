#ifndef RAVENENGINE_CAMERA_HPP
#define RAVENENGINE_CAMERA_HPP


#include "Component.hpp"
#include "math/Vector3.hpp"
#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"


namespace re {

    // TODO: Do changes in Camera component
    /**
     * @brief Camera render class
     */
    class Camera : public Component {
    public:
        /**
         * @brief Camera Type
         */
        enum Type {
            DIRECTION = 1,
            LOOK_AT = 2
        };

    public:
        explicit Camera(Type type);

        /**
         *
         * @param owner valid pointer to entity that owns this component
         */
        explicit Camera(Type type, Entity* owner);

        /**
         * @brief Construct Camera from JSON
         * @param component json object
         * @param owner valid pointer to entity that owns this component
         */
        Camera(json& component, Entity* owner);

        /**
         * @brief Define projection matrix as Orthographic
         * @param left
         * @param right
         * @param top
         * @param bottom
         * @param near
         * @param far
         */
        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        // TODO: Change camera direction to use Quaternions
        /**
         * @brief Define projection matrix as Perspective
         * @param fovy
         * @param aspect
         * @param near
         * @param far
         */
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        /**
         * @brief Set View to a specific rotation
         * @param position
         * @param rotation Quaternion with orientation
         */
        void setViewDirection(const vec3 &position, const quat &rotation);

        /**
         * @brief Set view to look at a target
         * @param position
         * @param target
         * @param up By default is {0, -1, 0}
         */
        void setViewTarget(const vec3& position, const vec3& target, const vec3& up = {0.f, -1.f, 0.f});

        /**
         * @brief Update View and Proj matrices;
         */
        void update();

        [[nodiscard]] const Matrix4& getProjection() const;

        [[nodiscard]] const Matrix4& getView() const;

        /**
         * @brief Serialize Camera to JSON
         *
         * This is not used so far
         */
        json serialize() override;

        /**
         * @brief Serialize Camera from JSON
         *
         * This is not used so far
         */
        void serialize(json &component) override;

    public:
        Type type{};

    private:
        Matrix4 projection{1.0f};
        Matrix4 view{1.0f};
    };


} // namespace re


#endif //RAVENENGINE_CAMERA_HPP
