#include "Material.hpp"

#include "AssetsManager.hpp"
#include "Texture.hpp"


namespace re {

    /**
     * @brief Construct Material from a GLTF2 file
     * @param model TinyGLTF model
     * @param material TinyGLTF material
     */
    Material::Material(const tinygltf::Model& model, const tinygltf::Material& material) {
        if (material.values.find("baseColorTexture") != material.values.end()) {
            tinygltf::Texture texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
            tinygltf::Image image = model.images[texture.source];

            Texture::Sampler sampler{};
            if (texture.sampler > -1) {
                tinygltf::Sampler smpl = model.samplers[texture.sampler];
                sampler.minFilter = Texture::Sampler::getVkFilterMode(smpl.minFilter);
                sampler.magFilter = Texture::Sampler::getVkFilterMode(smpl.magFilter);
                sampler.addressModeU = Texture::Sampler::getVkWrapMode(smpl.wrapS);
                sampler.addressModeV = Texture::Sampler::getVkWrapMode(smpl.wrapT);
                sampler.addressModeW = sampler.addressModeV;
            }

            textures[TextureType::BASE] = AssetsManager::getInstance()->add<Texture>(image.name, AssetsManager::getInstance()->getDevice(), image.uri, sampler);
            texCoordSets.baseColor = material.pbrMetallicRoughness.baseColorTexture.texCoord;
        }

        if (material.values.find("baseColorFactor") != material.values.end()) {
            baseColorFactor = vec4(material.pbrMetallicRoughness.baseColorFactor.data());
        }
    }

    Material::~Material() = default;

} // namespace re
