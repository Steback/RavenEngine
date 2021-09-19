#include "Material.hpp"

#include "AssetsManager.hpp"


namespace re {

    Material::Material(const tinygltf::Model& model, const tinygltf::Material& material) {
        if (material.values.find("baseColorTexture") != material.values.end()) {
            textures[TextureType::BASE] = AssetsManager::getInstance()->addTexture(model, model.textures[material.pbrMetallicRoughness.baseColorTexture.index]);
            texCoordSets.baseColor = material.pbrMetallicRoughness.baseColorTexture.texCoord;
        }

        if (material.values.find("baseColorFactor") != material.values.end()) {
            baseColorFactor = vec4(material.pbrMetallicRoughness.baseColorFactor.data());
        }
    }

    Material::~Material() = default;

} // namespace re
