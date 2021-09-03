#include "DamagedHelmet.hpp"

#include "math/Common.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"



DamagedHelmet::DamagedHelmet() : re::Base("DamagedHelmet") {
    scene->loadScene("scenes/damagedHelmet.json", assetsManager.get());
    renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model", scene.get(), assetsManager.get());

    entity = scene->getEntity("Damaged Helmet");
}

DamagedHelmet::~DamagedHelmet() = default;

void DamagedHelmet::onUpdate() {

}

re::vec3 angles;
void DamagedHelmet::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        ImGui::Text("Entity name %s", entity->getName().c_str());

        auto& transform = entity->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());
        ImGui::DragFloat3(fmt::format("{} Angles", entity->getName()).c_str(), angles.ptr(), 0.1f);
        ImGui::DragFloat4(fmt::format("{} Rotation", entity->getName()).c_str(), transform.rotation.ptr(), 0.1f);

        ImGui::Separator();

        auto camera = renderSystem->getCamera();
        auto& transformCamera = camera->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", camera->getName()).c_str(), transformCamera.position.ptr());
        ImGui::InputFloat2(fmt::format("{} Scale", camera->getName()).c_str(), transformCamera.scale.ptr());
        ImGui::DragFloat4(fmt::format("{} Rotation", camera->getName()).c_str(), transformCamera.rotation.ptr(), 0.1f);

    }
    ImGui::End();
}
