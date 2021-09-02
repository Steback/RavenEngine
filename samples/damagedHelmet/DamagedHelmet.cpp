#include "DamagedHelmet.hpp"

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

void DamagedHelmet::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        ImGui::Text("Entity name %s", entity->getName().c_str());

        auto& transform = entity->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", entity->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", entity->getName()).c_str(), transform.scale.ptr());

        re::quat rotation = transform.rotation;
        ImGui::DragFloat4(fmt::format("{} Rotation", entity->getName()).c_str(), rotation.ptr(), 0.1f);
        rotation.normalise();
        transform.rotation = rotation;
    }
    ImGui::End();
}
