#ifndef RAVENENGINE_ASSETSMANAGER_HPP
#define RAVENENGINE_ASSETSMANAGER_HPP


#include <memory>
#include <unordered_map>
#include <string>

#include "tiny_gltf.h"
#include "stb_image.h"

#include "Mesh.hpp"


namespace re {

    class Device;
    class Model;
    class Texture;
    class Material;
    class Skybox;

    enum DescriptorSetType {
        UBO = 1,
        TEXTURE = 2
    };

    /**
     * @brief Assets Manager class\n
     */
    class AssetsManager : NonCopyable {
        friend class Base;

        explicit AssetsManager(std::shared_ptr<Device> device);

        static void setup(std::shared_ptr<Device> device);

    public:
        ~AssetsManager() override;

        static AssetsManager* getInstance();

        void setupDescriptorsPool(uint32_t imageCount);

        void allocateDescriptorSet(DescriptorSetType type,  VkDescriptorSet* set);

        VkDescriptorSetLayout getMaterialLayout();

        VkDescriptorSetLayout getUboLayout();

        VkDescriptorSetLayout getTextureLayout();

        /**
         *
         * @param fileName Name of GLTF2 file
         * @param name Optional. Specific name to save model. Bu default is empty.
         */
        std::shared_ptr<Model> loadModel(const std::string& fileName, const std::string& name = "");

        std::shared_ptr<Model> getModel(uint32_t name);

        std::shared_ptr<Model> getModel(const std::string& name);

        /**
         * @brief Load Mesh from GLTF Mesh
         * @param model TinyGLTF Model
         * @param node  TinyGLTF node
         * @return shared_ptr of created Mesh
         */
        std::shared_ptr<Mesh> addMesh(const tinygltf::Model& model, const tinygltf::Node& node);

        std::shared_ptr<Mesh> getMesh(uint32_t name);

        std::shared_ptr<Mesh> getMesh(const std::string& name);

        /**
         * @brief Add Texture to AssetsManager
         * @param texture TinyGLTF Texture
         * @return Pointer to Texture
         */
        std::shared_ptr<Texture> addTexture(const tinygltf::Model& model, const tinygltf::Texture &texture);

        std::shared_ptr<Texture> getTexture(uint32_t id);

        std::shared_ptr<Texture> getTexture(const std::string& name);

        std::shared_ptr<Material> addMaterial(const tinygltf::Model& gltfModel, const tinygltf::Material &gltfMaterial);

        std::shared_ptr<Material> getMaterial(uint32_t id);

        std::shared_ptr<Material> getMaterial(const std::string& name);

        /**
         *
         * @param name File name in .ktx format
         */
        std::unique_ptr<Skybox> loadSkybox(const std::string &name, VkRenderPass renderPass);

    private:
        AssetsManager() = default;

        void setupDescriptorSetsLayout();

        void setupMaterialDescriptorsSets();

    private:
        static AssetsManager* singleton;
        VkDescriptorPool descriptorPool{};
        VkDescriptorSetLayout materialSetLayout{};
        VkDescriptorSetLayout uboSetLayout{};
        VkDescriptorSetLayout textureSetLayout{};
        std::shared_ptr<Device> device;
        std::unordered_map<uint32_t, std::shared_ptr<Model>> models;
        std::unordered_map<uint32_t, std::shared_ptr<Mesh>> meshes;
        std::unordered_map<uint32_t, std::shared_ptr<Texture>> textures;
        std::unordered_map<uint32_t, std::shared_ptr<Material>> materials;
    };

} // namespace re


#endif //RAVENENGINE_ASSETSMANAGER_HPP
