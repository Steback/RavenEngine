#include "Cube.hpp"


Cube::Cube() : re::Base("Cube") {
    auto cube = assetsManager->loadModel("models/Box.gltf");
    auto entity = scene->addEntity("Cube");
}

Cube::~Cube() = default;

void Cube::onUpdate() {

}
