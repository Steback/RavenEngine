#include "Sponza.hpp"

#include "math/Common.hpp"
#include "math/Vector3.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"


Sponza::Sponza(CLI::App &app) : re::Base("Sponza", app) {
    loadScene("scenes/sponza.json");
}

Sponza::~Sponza() = default;

void Sponza::onUpdate() {
    if (!camera) return;

    auto& transform = camera->getComponent<re::Transform>();

    re::vec2 cursorOffset = re::input::getCursorOffset();
    if (re::input::getKey(re::input::KEY_LEFT_SHIFT) && re::input::getKey(re::input::KEY_LEFT_ALT) && re::input::getMouseButton(re::input::MouseButton::LEFT)) {

    } else if (re::input::getKey(re::input::KEY_LEFT_ALT) && re::input::getMouseButton(re::input::MouseButton::LEFT)) {
        float yaw = cursorOffset.x;
        float pitch = cursorOffset.y;
        re::vec3 direction;
        direction.x = std::cos(yaw) * std::cos(pitch);
        direction.y = std::sin(pitch);
        direction.x = std::sin(yaw) * std::cos(pitch);
        transform.rotation += lockSpeed * re::time::deltaTime() * re::quat(direction);
        transform.rotation.normalise();
    }
}

void Sponza::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        if (!entity) return;

        auto& transform = entity->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());
        ImGui::InputFloat3(fmt::format("{} Rotation", entity->getName()).c_str(), eulerAngles.ptr());
        transform.rotation = re::quat(re::radians(eulerAngles));

        ImGui::Separator();

        auto& cameraTransform = camera->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", camera->getName()).c_str(), cameraTransform.position.ptr());

        re::vec3 cameraAngles = re::degrees(cameraTransform.getEulerAngles());
        ImGui::Text("%s", fmt::format("{} Angles: x: {} - y: {} - z: {}", camera->getName(), cameraAngles.x, cameraAngles.y, cameraAngles.z).c_str());

        auto& cameraComponent = camera->getComponent<re::Camera>();

        ImGui::Separator();

        re::vec2 cursorPosition = re::input::getCursorPosition();
        ImGui::Text("%s", fmt::format("Cursor position X: {} - Y: {}", cursorPosition.x, cursorPosition.y).c_str());

        re::vec2 cursorOffset = re::input::getCursorOffset();
        ImGui::Text("%s", fmt::format("Cursor offset X: {} - Y: {}", cursorOffset.x, cursorOffset.y).c_str());

        ImGui::Separator();

        if (ImGui::Button("Save")) {
            re::jobs::submit([scene = scene]() {
                scene->save();
            });
        }
    }
    ImGui::End();
}

void Sponza::onLoadScene() {
    entity = scene->getEntity("Sponza");
    camera = scene->getEntity("Camera");
    eulerAngles = re::degrees(entity->getComponent<re::Transform>().getEulerAngles());
}
