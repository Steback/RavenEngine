#ifndef RAVENENGINE_CAMERA_HPP
#define RAVENENGINE_CAMERA_HPP


#include "Component.hpp"
#include "engine/math/Math.hpp"
#include "engine/math/Vector3.hpp"
#include "engine/math/Matrix4.hpp"
#include "engine/math/Quaternion.hpp"


namespace re {

    class Camera : public Component {
    public:
        explicit Camera(float fov, float zNear, float zFar, const Vector3& target, Entity* owner);

        Camera(json& component, Entity* owner);

        void setOrthographic(float left, float right, float top, float bottom);

        void setPerspective(float aspect);

        void lookAt(const Vector3& up = {0.0f, 1.0f, 0.0f});

        void update();

        json serialize() override;

        void serialize(json &component) override;

    public:
        float fov{Math::deg2rad(45.0f)};
        float zNear{0.1f}, zFar{100};
        Vector3 target;
        Matrix4 projection{1.0f};
        Matrix4 view{1.0f};
    };


} // namespace re


#endif //RAVENENGINE_CAMERA_HPP
