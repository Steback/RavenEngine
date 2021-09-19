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
        Material(const tinygltf::Model& model, const tinygltf::Material& material);

        ~Material();

    public:
        vec4 baseColorFactor{1.0f};
        std::unordered_map<TextureType, std::shared_ptr<Texture>> textures;
        TexCoordSets texCoordSets;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    };

} // namespace re


#endif //RAVENENGINE_MATERIAL_HPP
