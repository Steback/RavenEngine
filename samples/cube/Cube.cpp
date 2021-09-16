#include "Cube.hpp"

#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


Cube::Cube() : re::Base("Cube") {
    loadScene("scenes/cube.json");
}

Cube::~Cube() = default;

void Cube::onUpdate() {

}

void Cube::onDrawImGui() {
    if (cube) {
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
        }
        ImGui::End();
    }
}

void Cube::onLoadScene() {
    cube = scene->getEntity("Cube");
}
