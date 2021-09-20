#include "AssetsManager.hpp"

#include "spdlog/spdlog.h"

#include "Model.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "render/Device.hpp"
#include "files/FilesManager.hpp"
#include "files/File.hpp"
#include "utils/Utils.hpp"
#include "render/buffers/Buffer.hpp"
#include "scene/Skybox.hpp"


namespace re {

    AssetsManager* AssetsManager::singleton;

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {
        textures[std::hash<std::string>()("empty")] = Texture::loadFromFile(this->device, "empty.png", Texture::Sampler{});
    }

    AssetsManager::~AssetsManager() {
        for (auto& [type, layout] : layouts)
            vkDestroyDescriptorSetLayout(device->getDevice(), layout, nullptr);

        vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);
    }

    AssetsManager* AssetsManager::getInstance() {
        return singleton;
    }

    void AssetsManager::setupDescriptorsPool(uint32_t imageCount) {
        // TODO: Find the way to do this dynamically
        uint32_t uboCount = 2;
        uint32_t standaloneTexCount = 1;
        uint32_t imageSamplerCount = 0;
        uint32_t materialCount = 0;

        for (auto& material : materials) {
            imageSamplerCount += 1;
            materialCount++;
        }

        std::vector<VkDescriptorPoolSize> poolSizes = {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, imageSamplerCount + standaloneTexCount},
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uboCount },
        };

        VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();

        poolInfo.maxSets = uboCount + standaloneTexCount + (materialCount);

        vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool);

        setupDescriptorSetsLayout();
        setupMaterialDescriptorsSets();
    }

    void AssetsManager::allocateDescriptorSet(DescriptorSetType type, VkDescriptorSet *set) {
        VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &layouts[type];
        vkAllocateDescriptorSets(device->getDevice(), &allocateInfo, set);
    }

    VkDescriptorSetLayout AssetsManager::getDescriptorSetLayout(DescriptorSetType type) {
        return layouts[type];
    }

    std::shared_ptr<Model> AssetsManager::loadModel(const std::string& fileName, const std::string& name) {
        File file = files::getFile(fileName);
        std::string modelName = name.empty() ? file.getName(true) : name;
        uint32_t nameHash = std::hash<std::string>()(modelName);

        if (models.find(nameHash) != models.end()) return models[nameHash];

        std::string error;
        std::string warning;

        tinygltf::Model gltfModel;
        tinygltf::TinyGLTF gltfContext;

        bool fileLoaded;
        if (file.getExtension() == ".glb")
            fileLoaded = gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, file.getPath());
        else
            fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, file.getPath());

        if (fileLoaded)
            return models[nameHash] = std::make_shared<Model>(fileName, gltfModel);
        else
            return nullptr;
    }

    std::shared_ptr<Model> AssetsManager::getModel(uint32_t name) {
        return models[name];
    }

    std::shared_ptr<Model> AssetsManager::getModel(const std::string &name) {
        return models[std::hash<std::string>()(name)];
    }

    std::shared_ptr<Mesh> AssetsManager::addMesh(const tinygltf::Model &model, const tinygltf::Node &node) {
        uint32_t meshID = std::hash<std::string>()(node.name);
        const tinygltf::Mesh& mesh = model.meshes[node.mesh];

        Mesh::Data data = Mesh::loadMesh(model, mesh);

        for (auto& primitive : mesh.primitives) {
            const tinygltf::Material& material = model.materials[primitive.material];
            return meshes[meshID] = std::make_shared<Mesh>(device, data, addMaterial(model, material));
        }

        return nullptr;
    }

    std::shared_ptr<Mesh> AssetsManager::getMesh(uint32_t name) {
        return meshes[name];
    }

    std::shared_ptr<Mesh> AssetsManager::getMesh(const std::string &name) {
        return meshes[std::hash<std::string>()(name)];
    }

    std::shared_ptr<Texture> AssetsManager::addTexture(const tinygltf::Model& model, const tinygltf::Texture &texture) {
        tinygltf::Image image = model.images[texture.source];
        uint32_t textureID = std::hash<std::string>()(image.name);

        if (textures.find(textureID) != textures.end()) return textures[textureID];

        Texture::Sampler sampler{};
        if (texture.sampler > -1) {
            tinygltf::Sampler smpl = model.samplers[texture.sampler];
            sampler.minFilter = Texture::Sampler::getVkFilterMode(smpl.minFilter);
            sampler.magFilter = Texture::Sampler::getVkFilterMode(smpl.magFilter);
            sampler.addressModeU = Texture::Sampler::getVkWrapMode(smpl.wrapS);
            sampler.addressModeV = Texture::Sampler::getVkWrapMode(smpl.wrapT);
            sampler.addressModeW = sampler.addressModeV;
        }

        return textures[textureID] = Texture::loadFromFile(device, image.uri, sampler);
    }

    std::shared_ptr<Texture> AssetsManager::getTexture(uint32_t id) {
        return textures[id];
    }

    std::shared_ptr<Texture> AssetsManager::getTexture(const std::string &name) {
        return textures[std::hash<std::string>()(name)];
    }

    std::shared_ptr<Material> AssetsManager::addMaterial(const tinygltf::Model &gltfModel, const tinygltf::Material &gltfMaterial) {
        return materials[std::hash<std::string>()(gltfMaterial.name)] = std::make_shared<Material>(gltfModel, gltfMaterial);
    }

    std::shared_ptr<Material> AssetsManager::getMaterial(uint32_t id) {
        return materials[id];
    }

    std::shared_ptr<Material> AssetsManager::getMaterial(const std::string &name) {
        return materials[std::hash<std::string>()(name)];
    }

    // TODO: Change how to create a Skybox
    std::unique_ptr<Skybox> AssetsManager::loadSkybox(const std::string &name, VkRenderPass renderPass) {
        auto model = loadModel("models/cube.gltf", "Skybox");
        auto texture = textures[std::hash<std::string>()("Skybox")] = Texture::loadCubeMap(device, name);
        texture->updateDescriptor();

        return std::make_unique<Skybox>(device, renderPass);
    }

    void AssetsManager::setupDescriptorSetsLayout() {
        // Materials Descriptor Set Layout
        std::vector<VkDescriptorSetLayoutBinding> materialBindings = {
                { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
        };

        VkDescriptorSetLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        layoutInfo.bindingCount = static_cast<uint32_t>(materialBindings.size());
        layoutInfo.pBindings = materialBindings.data();

        vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &layouts[MATERIAL]);

        // UBO Descriptor Set Layout
        std::vector<VkDescriptorSetLayoutBinding> uboBindings = {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT}
        };

        layoutInfo.bindingCount = static_cast<uint32_t>(uboBindings.size());
        layoutInfo.pBindings = uboBindings.data();
        vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &layouts[UBO]);

        // Standalone Texture Descriptor Set Layout
        std::vector<VkDescriptorSetLayoutBinding> bindings = {
                {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}
        };

        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();
        vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &layouts[TEXTURE]);
    }

    void AssetsManager::setupMaterialDescriptorsSets() {
        for (auto& [id, material] : materials) {
            VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
            descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocInfo.descriptorPool = descriptorPool;
            descriptorSetAllocInfo.pSetLayouts = &layouts[MATERIAL];
            descriptorSetAllocInfo.descriptorSetCount = 1;

            checkResult(vkAllocateDescriptorSets(device->getDevice(), &descriptorSetAllocInfo, &material->descriptorSet),
                        "Failed to allocate material descriptor sets");

            auto empty = textures[std::hash<std::string>()("empty")]->descriptor;
            std::vector<VkDescriptorImageInfo> imageDescriptors = {
                    empty,
            };

            if (material->textures[Material::BASE]) {
                imageDescriptors[0] = material->textures[Material::BASE]->descriptor;
            }

            std::array<VkWriteDescriptorSet, 1> writeDescriptorSets{};
            for (size_t i = 0; i < imageDescriptors.size(); i++) {
                writeDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSets[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                writeDescriptorSets[i].descriptorCount = 1;
                writeDescriptorSets[i].dstSet = material->descriptorSet;
                writeDescriptorSets[i].dstBinding = static_cast<uint32_t>(i);
                writeDescriptorSets[i].pImageInfo = &imageDescriptors[i];
            }

            vkUpdateDescriptorSets(device->getDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
        }
    }

} // namespace re