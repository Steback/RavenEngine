#include "Sponza.hpp"

#include "math/Math.hpp"
#include "math/Vector3.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"

using namespace re;


Sponza::Sponza(CLI::App &app) : Base("Sponza", app) {
    loadScene("scenes/sponza.json");
}

Sponza::~Sponza() = default;

void Sponza::onUpdate() {
    if (!camera) return;

    auto& transform = camera->getComponent<Transform>();
    vec2 cursorOffset = input::getCursorOffset();

    if (input::getKey(input::KEY_LEFT_SHIFT) && input::getKey(input::KEY_LEFT_ALT) && input::getMouseButton(input::MOUSE_BUTTON_LEFT)) {

    } else if (input::getKey(input::KEY_LEFT_ALT) && input::getMouseButton(input::MOUSE_BUTTON_LEFT)) {

    }
}

void Sponza::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        if (!entity) return;

        {
            auto& transform = entity->getComponent<Transform>();
            ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.values);
            ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.values);
            ImGui::InputFloat3(fmt::format("{} Rotation", entity->getName()).c_str(), eulerAngles.values);
            vec3 angles = Math::deg2rad(eulerAngles);
            transform.rotation = quat(Vector3{angles.y, angles.z, angles.x});
            ImGui::Text("%s", transform.rotation.str().c_str());
        }
        ImGui::Separator();
        {
            auto& cameraTransform = camera->getComponent<Transform>();
            ImGui::InputFloat3(fmt::format("{} Position", camera->getName()).c_str(), cameraTransform.position.values);

            vec3 cameraAngles = Math::rad2deg(cameraTransform.getEulerAngles());
            ImGui::Text("%s", fmt::format("{} Angles: {}", camera->getName(), cameraAngles.str()).c_str());

            auto& cameraComponent = camera->getComponent<Camera>();
            float fov = Math::rad2deg(cameraComponent.fov);
            ImGui::InputFloat(fmt::format("{} FOV", camera->getName()).c_str(), &fov);
            cameraComponent.fov = Math::deg2rad(fov);

            ImGui::InputFloat(fmt::format("{} Near Plane", camera->getName()).c_str(), &cameraComponent.zNear);
            ImGui::InputFloat(fmt::format("{} Far Plane", camera->getName()).c_str(), &cameraComponent.zFar);
        }
        ImGui::Separator();
        {
            vec2 cursorPosition = input::getCursorPosition();
            ImGui::Text("%s", fmt::format("Cursor position: {}", cursorPosition.str()).c_str());

            vec2 cursorOffset = input::getCursorOffset();
            ImGui::Text("%s", fmt::format("Cursor offset: {}", cursorOffset.str()).c_str());
        }
        ImGui::Separator();
        {
            if (ImGui::Button("Save")) {
                jobs::submit([scene = scene]() {
                    scene->save();
                });
            }
        }
    }
    ImGui::End();
}

void Sponza::onLoadScene() {
    entity = scene->getEntity("Sponza");
    camera = scene->getEntity("Camera");
    cameraTarget = camera->getComponent<Camera>().target;
    Vector3 angles = Math::rad2deg(entity->getComponent<Transform>().getEulerAngles());
    eulerAngles = {angles.z, angles.x, angles.y};
}
