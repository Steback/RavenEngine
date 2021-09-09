#include "Material.hpp"

#include "AssetsManager.hpp"


namespace re {

    Material::Material(AssetsManager* assetsManager, const tinygltf::Model& model, const tinygltf::Material& material) {
        if (material.values.find("baseColorTexture") != material.values.end()) {
            baseColorTexture = assetsManager->addTexture(model, model.textures[material.pbrMetallicRoughness.baseColorTexture.index]);
            texCoordSets.baseColor = material.pbrMetallicRoughness.baseColorTexture.texCoord;
        }

        if (material.values.find("metallicRoughnessTexture") != material.values.end()) {
            metallicRoughnessTexture = assetsManager->addTexture(model, model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index]);
            texCoordSets.metallicRoughness = material.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;
        }

        if (material.values.find("roughnessFactor") != material.values.end()) {
            roughnessFactor = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
        }

        if (material.values.find("metallicFactor") != material.values.end()) {
            metallicFactor = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
        }

        if (material.values.find("baseColorFactor") != material.values.end()) {
            baseColorFactor = vec4(material.pbrMetallicRoughness.baseColorFactor.data());
        }

        if (material.additionalValues.find("normalTexture") != material.additionalValues.end()) {
            normalTexture = assetsManager->addTexture(model, model.textures[material.normalTexture.index]);
            texCoordSets.normal = material.normalTexture.texCoord;
        }

        if (material.additionalValues.find("emissiveTexture") != material.additionalValues.end()) {
            emissiveTexture = assetsManager->addTexture(model, model.textures[material.emissiveTexture.index]);
            texCoordSets.emissive = material.emissiveTexture.texCoord;
        }

        if (material.additionalValues.find("occlusionTexture") != material.additionalValues.end()) {
            occlusionTexture = assetsManager->addTexture(model, model.textures[material.occlusionTexture.index]);
            texCoordSets.occlusion = material.occlusionTexture.texCoord;
        }

        if (material.additionalValues.find("alphaMode") != material.additionalValues.end()) {
            if (material.alphaMode == "BLEND") {
                alphaMode = AlphaMode::BLEND;
            }
            if (material.alphaMode == "MASK") {
                alphaCutoff = 0.5f;
                alphaMode = AlphaMode::MASK;
            }
        }

        if (material.additionalValues.find("alphaCutoff") != material.additionalValues.end()) {
            alphaCutoff = static_cast<float >(material.alphaCutoff);
        }

        if (material.additionalValues.find("emissiveFactor") != material.additionalValues.end()) {
            vec3 temp(material.emissiveFactor.data());
            // TODO: Create Vec4(vec3, float) constructor
            emissiveFactor = vec4(temp.x, temp.y, temp.z, 1.0f);
        }

        // TODO: Find out if there is a nicer way of reading these properties with recent tinygltf headers
        if (material.extensions.find("KHR_materials_pbrSpecularGlossiness") != material.extensions.end()) {
            auto ext = material.extensions.find("KHR_materials_pbrSpecularGlossiness");
            if (ext->second.Has("specularGlossinessTexture")) {
                auto index = ext->second.Get("specularGlossinessTexture").Get("index").Get<int>();
                extension.specularGlossinessTexture = assetsManager->addTexture(model, model.textures[index]);

                auto texCoordSet = ext->second.Get("specularGlossinessTexture").Get("texCoord").Get<int>();
                texCoordSets.specularGlossiness = texCoordSet;
                pbrWorkflows.specularGlossiness = true;
            }

            if (ext->second.Has("diffuseTexture")) {
                auto index = ext->second.Get("diffuseTexture").Get("index").Get<int>();
                extension.diffuseTexture = assetsManager->addTexture(model, model.textures[index]);
            }

            if (ext->second.Has("diffuseFactor")) {
                auto factor = ext->second.Get("diffuseFactor");
                for (uint32_t i = 0; i < factor.ArrayLen(); i++) {
                    const auto& val = factor.Get(i);
                    extension.diffuseFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
                }

            }

            if (ext->second.Has("specularFactor")) {
                auto factor = ext->second.Get("specularFactor");
                for (uint32_t i = 0; i < factor.ArrayLen(); i++) {
                    const auto& val = factor.Get(i);
                    extension.specularFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
                }
            }
        }
    }

    Material::~Material() = default;

} // namespace re
