#include "Cube.hpp"

#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


Cube::Cube() : re::Base("Cube") {
    auto cube = assetsManager->loadModel("models/Box.gltf");
    auto entity = scene->addEntity("Cube");
    entity->addComponent<re::Transform>(re::vec3{0.0f, 0.0f, 2.5f}, re::vec3{0.5f, 0.5f, 0.5f}, re::vec3{});
    entity->addComponent<re::MeshRender>(cube);
}

Cube::~Cube() = default;

void Cube::onUpdate() {
    renderSystem->setView();
}

void Cube::onDrawImGui() {
   ImGui::ShowDemoWindow();
}
