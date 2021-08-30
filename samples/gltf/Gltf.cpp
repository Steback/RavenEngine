#include "Gltf.hpp"

#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"



Gltf::Gltf() : re::Base("Gltf") {
    scene->loadScene("scenes/gltf.json", assetsManager.get());
    renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "samples/cube/model", *scene);

    suzanne = scene->getEntity("Suzanne");
}

Gltf::~Gltf() = default;

void Gltf::onUpdate() {

}

void Gltf::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        ImGui::Text("Entity name %s", suzanne->getName().c_str());

        auto& transform = suzanne->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", suzanne->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", suzanne->getName()).c_str(), transform.scale.ptr());

        re::quat rotation = transform.rotation;
        ImGui::DragFloat4(fmt::format("{} Rotation", suzanne->getName()).c_str(), rotation.ptr(), 0.1f);
        rotation.normalise();
        transform.rotation = rotation;
    }
    ImGui::End();
}
