#ifndef RAVENENGINE_MATERIAL_HPP
#define RAVENENGINE_MATERIAL_HPP


#include <memory>

#include "vulkan/vulkan.h"
#include "tiny_gltf.h"

#include "Asset.hpp"
#include "engine/math/Vector3.hpp"
#include "engine/math/Vector4.hpp"


namespace re {

    class Texture;

    class Material : public Asset {

    public:
        enum AlphaMode {
            OPAQUE,
            MASK,
            BLEND
        };

        enum TextureType {
            BASE = 0
        };

        struct TexCoordSets {
            uint8_t baseColor{};
        };

        struct PushConstantBlock {
            vec4 baseColorFactor;
            int colorTextureSet;
        };

    public:
        Material(std::string name, const tinygltf::Model& model, const tinygltf::Material& material);

        ~Material() override;

    public:
        vec4 baseColorFactor{1.0f};
        std::unordered_map<TextureType, Texture*> textures;
        TexCoordSets texCoordSets;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    };

} // namespace re


#endif //RAVENENGINE_MATERIAL_HPP
