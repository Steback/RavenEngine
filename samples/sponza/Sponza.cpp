#include "Sponza.hpp"

#include "math/Common.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"


Sponza::Sponza(CLI::App &app) : re::Base("Sponza", app) {
    loadScene("scenes/sponza.json");
}

Sponza::~Sponza() = default;

void Sponza::onUpdate() {

}

void Sponza::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        if (entity) {
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
    }
    ImGui::End();
}

void Sponza::onLoadScene() {
    entity = scene->getEntity("Sponza");
    camera = scene->getEntity("Camera");
    eulerAngles = re::degrees(entity->getComponent<re::Transform>().getEulerAngles());
}
