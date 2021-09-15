#ifndef RAVENENGINE_MATERIAL_HPP
#define RAVENENGINE_MATERIAL_HPP


#include <memory>

#include "vulkan/vulkan.h"
#include "tiny_gltf.h"

#include "math/Vector3.hpp"
#include "math/Vector4.hpp"


namespace re {

    class Texture;
    class AssetsManager;

    /**
     * @brief Material wrapper class
     */
    class Material {
        friend AssetsManager;

    public:
        enum AlphaMode{ OPAQUE, MASK, BLEND };

        enum PBRWorkflows{ METALLIC_ROUGHNESS = 0, SPECULAR_GLOSINESS = 1 };

        struct TexCoordSets {
            uint8_t baseColor{};
            uint8_t metallicRoughness{};
            uint8_t specularGlossiness{};
            uint8_t normal{};
            uint8_t occlusion{};
            uint8_t emissive{};
        };

        struct Extension {
            std::shared_ptr<Texture> specularGlossinessTexture;
            std::shared_ptr<Texture> diffuseTexture;
            vec4 diffuseFactor{1.0f};
            vec3 specularFactor{0.0f};
        };

        struct PbrWorkflows {
            bool metallicRoughness{true};
            bool specularGlossiness{false};
        };

    public:
        Material(const tinygltf::Model& model, const tinygltf::Material& material);

        ~Material();

    // TODO: Should this keep as public?
    public:
        AlphaMode alphaMode = OPAQUE;
        float alphaCutoff{1.0f};
        float metallicFactor{1.0f};
        float roughnessFactor{1.0f};
        vec4 baseColorFactor{1.0f};
        vec4 emissiveFactor{1.0f};
        std::shared_ptr<Texture> baseColorTexture;
        std::shared_ptr<Texture> metallicRoughnessTexture;
        std::shared_ptr<Texture> normalTexture;
        std::shared_ptr<Texture> occlusionTexture;
        std::shared_ptr<Texture> emissiveTexture;
        TexCoordSets texCoordSets;
        Extension extension;
        PbrWorkflows pbrWorkflows;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    };

} // namespace re


#endif //RAVENENGINE_MATERIAL_HPP
