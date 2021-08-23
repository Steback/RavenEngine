#include "Cube.hpp"


Cube::Cube() : re::Base("Cube") {
    auto cube = assetsManager->loadModel("models/Box.gltf");
}

Cube::~Cube() = default;

void Cube::onUpdate() {

}
