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
    re::vec3 rotation = transform.getEulerAngles();
    re::vec3 translation = transform.position;

    re::vec3 rotate;
    if (re::input::getKey(re::input::KEY_UP)) rotate.y += 1.0f;
    if (re::input::getKey(re::input::KEY_DOWN)) rotate.y -= 1.0f;
    if (re::input::getKey(re::input::KEY_RIGHT)) rotate.x += 1.0f;
    if (re::input::getKey(re::input::KEY_LEFT)) rotate.x -= 1.0f;

    if (rotate * rotate > std::numeric_limits<float>::epsilon())
        rotation += lockSpeed * re::time::deltaTime() * rotate.normal();

    rotation.x = std::clamp(rotation.x, -1.5f, 1.5f);
    rotation.y = re::mod(rotation.y, re::twoPi());

    float yaw = rotation.y;
    const re::vec3 forwardDir{std::sin(yaw), 0.f, std::cos(yaw)};
    const re::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const re::vec3 upDir{0.f, -1.f, 0.f};

    re::vec3 moveDir{0.f};
    if (re::input::getKey(re::input::KEY_W)) moveDir += forwardDir;
    if (re::input::getKey(re::input::KEY_S)) moveDir -= forwardDir;
    if (re::input::getKey(re::input::KEY_D)) moveDir += rightDir;
    if (re::input::getKey(re::input::KEY_A)) moveDir -= rightDir;
//    if (input.getKey(KeyMappings::moveUp)) moveDir += upDir;
//    if (input.getKey(KeyMappings::moveDown)) moveDir -= upDir;

    if (moveDir * moveDir > std::numeric_limits<float>::epsilon())
        translation += moveSpeed * re::time::deltaTime() * moveDir.normal();

    transform.position = translation;
    transform.rotation = re::quat(rotation);
}

void Sponza::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        if (entity) return;

        auto& transform = entity->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());
        ImGui::InputFloat3(fmt::format("{} Rotation", entity->getName()).c_str(), eulerAngles.ptr());
        transform.rotation = re::quat(re::radians(eulerAngles));

        ImGui::Separator();

        auto& cameraTransform = camera->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", camera->getName()).c_str(), cameraTransform.position.ptr());

        re::vec3 cameraAngles = re::degrees(cameraTransform.getEulerAngles());
        ImGui::InputFloat3(fmt::format("{} Rotation", camera->getName()).c_str(), cameraAngles.ptr());
        cameraTransform.rotation = re::quat(re::radians(re::radians(cameraAngles)));

        auto& cameraComponent = camera->getComponent<re::Camera>();

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
