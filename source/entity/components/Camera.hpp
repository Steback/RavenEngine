#ifndef RAVENENGINE_CAMERA_HPP
#define RAVENENGINE_CAMERA_HPP


#include "Component.hpp"
#include "math/Vector3.hpp"
#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"


namespace re {

    // TODO: Do changes in Camera component and add Doxygen comments
    class Camera : public Component {
    public:
        enum Type {
            DIRECTION = 1,
            LOOK_AT = 2
        };

    public:
        explicit Camera(Type type);

        explicit Camera(Type type, Entity* owner);

        Camera(json& component, Entity* owner);

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        void setViewDirection(const vec3 &position, const quat &rotation);

        void setViewTarget(const vec3& position, const vec3& target, const vec3& up = {0.f, -1.f, 0.f});

        void update();

        [[nodiscard]] const Matrix4& getProjection() const;

        [[nodiscard]] const Matrix4& getView() const;

        json serialize() override;

        void serialize(json &component) override;

    public:
        Type type{};

    private:
        Matrix4 projection{1.0f};
        Matrix4 view{1.0f};
    };


} // namespace re


#endif //RAVENENGINE_CAMERA_HPP
