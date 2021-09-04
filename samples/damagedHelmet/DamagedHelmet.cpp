#include "DamagedHelmet.hpp"

#include "math/Common.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"



DamagedHelmet::DamagedHelmet() : re::Base("DamagedHelmet") {
    scene = std::make_shared<re::Scene>("damagedHelmet.json", assetsManager);
    renderSystem->setScene(scene);

    re::JobSystem::submit([scene = scene](){
        scene->load();
    });
}

DamagedHelmet::~DamagedHelmet() = default;

void DamagedHelmet::onUpdate() {
    if (!entity && scene->loaded()) {
        entity = scene->getEntity("Damaged Helmet");
        auto& transform = entity->getComponent<re::Transform>();
        angles = re::degrees(transform.rotation.getEulerAngles());
    }
}

void DamagedHelmet::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        if (scene->loaded()) {
            auto& transform = entity->getComponent<re::Transform>();
            ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
            ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());
            ImGui::DragFloat3(fmt::format("{} Angles", entity->getName()).c_str(), angles.ptr(), 0.1f);
            ImGui::DragFloat4(fmt::format("{} Rotation", entity->getName()).c_str(), transform.rotation.ptr(), 0.1f);
            transform.rotation = re::Quaternion(re::radians(angles));

            if (ImGui::Button("Save")) {
                scene->save();
            }
        }
    }
    ImGui::End();
}
