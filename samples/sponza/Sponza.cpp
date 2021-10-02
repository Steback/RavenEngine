#include "Sponza.hpp"

#include "math/Common.hpp"
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
    if (input::getKey(input::KEY_LEFT_SHIFT) && input::getKey(input::KEY_LEFT_ALT) && input::getMouseButton(input::MouseButton::LEFT)) {

    } else if (input::getKey(input::KEY_LEFT_ALT) && input::getMouseButton(input::MouseButton::LEFT)) {
        float yaw = cursorOffset.x;
        float pitch = cursorOffset.y;
        vec3 direction;
        direction.x = std::cos(yaw) * std::cos(pitch);
        direction.y = std::sin(pitch);
        direction.x = std::sin(yaw) * std::cos(pitch);
        transform.rotation += lockSpeed * time::deltaTime() * quat(direction);
        transform.rotation.normalise();
    }
}

void Sponza::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        if (!entity) return;

        auto& transform = entity->getComponent<Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());
        ImGui::InputFloat3(fmt::format("{} Rotation", entity->getName()).c_str(), eulerAngles.ptr());
        transform.rotation = quat(radians(eulerAngles));

        ImGui::Separator();

        auto& cameraTransform = camera->getComponent<Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", camera->getName()).c_str(), cameraTransform.position.ptr());

        vec3 cameraAngles = degrees(cameraTransform.getEulerAngles());
        ImGui::Text("%s", fmt::format("{} Angles: x: {} - y: {} - z: {}", camera->getName(), cameraAngles.x, cameraAngles.y, cameraAngles.z).c_str());

        auto& cameraComponent = camera->getComponent<Camera>();

        ImGui::Separator();

        vec2 cursorPosition = input::getCursorPosition();
        ImGui::Text("%s", fmt::format("Cursor position X: {} - Y: {}", cursorPosition.x, cursorPosition.y).c_str());

        vec2 cursorOffset = input::getCursorOffset();
        ImGui::Text("%s", fmt::format("Cursor offset X: {} - Y: {}", cursorOffset.x, cursorOffset.y).c_str());

        ImGui::Separator();

        if (ImGui::Button("Save")) {
            jobs::submit([scene = scene]() {
                scene->save();
            });
        }
    }
    ImGui::End();
}

void Sponza::onLoadScene() {
    entity = scene->getEntity("Sponza");
    camera = scene->getEntity("Camera");
    eulerAngles = degrees(entity->getComponent<Transform>().getEulerAngles());
}
