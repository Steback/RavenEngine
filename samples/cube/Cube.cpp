#include "Cube.hpp"

#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


Cube::Cube() : re::Base("Cube") {
    scene->loadScene("cube.json", assetsManager.get());
    renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model", *scene);

    cube = scene->getEntity("Cube");
}

Cube::~Cube() = default;

void Cube::onUpdate() {
    renderSystem->setView();
}

void Cube::onDrawImGui() {
    ImGui::SetNextWindowSize({-1, -1});
    ImGui::Begin("Debug Window");
    {
        ImGui::Text("Entity name %s", cube->getName().c_str());

        auto& transform = cube->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", cube->getName()).c_str(), transform.position.ptr());
        ImGui::InputFloat3(fmt::format("{} Size", cube->getName()).c_str(), transform.scale.ptr());

        re::quat rotation = transform.rotation;
        ImGui::DragFloat4(fmt::format("{} Rotation", cube->getName()).c_str(), rotation.ptr(), 0.1f);
        rotation.normalise();
        transform.rotation = rotation;

        ImGui::Separator();

        auto camera = renderSystem->getCamera();
        auto& transformCamera = camera->getComponent<re::Transform>();
        ImGui::InputFloat3(fmt::format("{} Position", camera->getName()).c_str(), transformCamera.position.ptr());
        ImGui::InputFloat2(fmt::format("{} Scale", camera->getName()).c_str(), transformCamera.scale.ptr());
        ImGui::DragFloat3(fmt::format("{} Rotation", camera->getName()).c_str(), transformCamera.eulerAngles.ptr(), 0.1f);
    }
    ImGui::End();
}
