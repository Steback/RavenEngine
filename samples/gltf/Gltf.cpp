#include "Gltf.hpp"


Gltf::Gltf() : re::Base("Gltf") {
    scene->loadScene("gltf.json", assetsManager.get());
    renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model", *scene);
}

Gltf::~Gltf() = default;

void Gltf::onUpdate() {

}

void Gltf::onDrawImGui() {

}
