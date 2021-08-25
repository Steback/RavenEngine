#include "Cube.hpp"

#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


Cube::Cube() : re::Base("Cube") {
    auto cube = assetsManager->loadModel("models/Box.gltf");
    auto entity = scene->addEntity("Cube");
    entity->addComponent<re::Transform>(re::vec3{}, re::vec3{}, re::vec3{});
    entity->addComponent<re::MeshRender>(cube);
}

Cube::~Cube() = default;

void Cube::onUpdate() {

}
