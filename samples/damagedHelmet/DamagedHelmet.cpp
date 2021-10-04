#include "DamagedHelmet.hpp"

#include "math/Math.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"
#include "entity/components/Light.hpp"



DamagedHelmet::DamagedHelmet(CLI::App& app) : re::Base("DamagedHelmet", app) {
    loadScene("scenes/damagedHelmet.json");
}

DamagedHelmet::~DamagedHelmet() = default;

void DamagedHelmet::onUpdate() {

}

void DamagedHelmet::onDrawImGui() {
    if (entity) {
        ImGui::SetNextWindowSize({-1, -1});
        ImGui::Begin("Debug Window");
        {
            auto& transform = entity->getComponent<re::Transform>();
            ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
            ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());
            ImGui::DragFloat3(fmt::format("{} Angles", entity->getName()).c_str(), angles.ptr(), 0.1f);
            ImGui::DragFloat4(fmt::format("{} Rotation", entity->getName()).c_str(), transform.rotation.ptr(), 0.1f);
            transform.rotation = re::Quaternion(re::radians(angles));

            ImGui::Separator();

            auto& lightTransform = light->getComponent<re::Transform>();
            auto& lightComponent = light->getComponent<re::Light>();
            ImGui::InputFloat3("Light Position", lightTransform.position.ptr());
            ImGui::ColorEdit3("Light Color", lightComponent.color.ptr());
            ImGui::InputFloat("Light Ambient", &lightComponent.ambient);

            ImGui::Separator();

            if (ImGui::Button("Save")) {
                re::jobs::submit([scene = scene]() {
                    scene->save();
                });
            }
        }
        ImGui::End();
    }
}

void DamagedHelmet::onLoadScene() {
    scene->loadSkybox("environments/cubemap_yokohama_rgba.ktx", renderer->getRenderPass());
    entity = scene->getEntity("Damaged Helmet");
    auto& transform = entity->getComponent<re::Transform>();
    angles = re::degrees(transform.rotation.getEulerAngles());
    light = scene->getEntity("Light");
}
