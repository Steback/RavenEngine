#include "AssetsManager.hpp"

#include "spdlog/spdlog.h"

#include "Model.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "render/Device.hpp"
#include "files/FilesManager.hpp"
#include "files/File.hpp"
#include "utils/Macros.hpp"
#include "render/Buffer.hpp"


namespace re {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {
        textures[std::hash<std::string>()("empty")] = Texture::loadFromFile(this->device, "empty.png", Texture::Sampler{});
    }

    AssetsManager::~AssetsManager() {
        vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device->getDevice(), descriptorSetLayout, nullptr);
    }

    std::shared_ptr<Model> AssetsManager::loadModel(const std::string &fileName) {
        File file = FilesManager::getFile(fileName.c_str());
        uint32_t nameHash = std::hash<std::string>()(file.getName(true));

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
            return models[nameHash] = std::make_shared<Model>(this, fileName, gltfModel);

        return nullptr;
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

    std::shared_ptr<Material> AssetsManager::addMaterial(const tinygltf::Model &gltfModel, const tinygltf::Material &gltfMaterial) {
        return materials[std::hash<std::string>()(gltfMaterial.name)] = std::make_shared<Material>(this, gltfModel, gltfMaterial);
    }

    void AssetsManager::setupDescriptors(uint32_t imageCount) {
        uint32_t imageSamplerCount = 0;
        uint32_t materialCount = 0;
        uint32_t meshCount = 0;

        for (auto& material : materials) {
            imageSamplerCount += 5;
            materialCount++;
        }

        for (auto& mesh : meshes)
            meshCount++;

        std::vector<VkDescriptorPoolSize> poolSizes = {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, imageSamplerCount * imageCount }
        };

        VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = (1 + materialCount + meshCount) * imageCount;

        vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool);

        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
                { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
                { 4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
        };

        VkDescriptorSetLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        layoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        layoutInfo.pBindings = setLayoutBindings.data();

        vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout);

        for (auto& [id, material] : materials) {
            VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
            descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocInfo.descriptorPool = descriptorPool;
            descriptorSetAllocInfo.pSetLayouts = &descriptorSetLayout;
            descriptorSetAllocInfo.descriptorSetCount = 1;

            RE_VK_CHECK_RESULT(vkAllocateDescriptorSets(device->getDevice(), &descriptorSetAllocInfo, &material->descriptorSet),
                               "Failed to allocate material descriptor sets");

            auto empty = textures[std::hash<std::string>()("empty")]->descriptor;
            std::vector<VkDescriptorImageInfo> imageDescriptors = {
                    empty,
                    empty,
                    material->normalTexture ? material->normalTexture->descriptor : empty,
                    material->occlusionTexture ? material->occlusionTexture->descriptor : empty,
                    material->emissiveTexture ? material->emissiveTexture->descriptor : empty
            };

            // TODO: glTF specs states that metallic roughness should be preferred, even if specular glosiness is present

            if (material->pbrWorkflows.metallicRoughness) {
                if (material->baseColorTexture) {
                    imageDescriptors[0] = material->baseColorTexture->descriptor;
                }
                if (material->metallicRoughnessTexture) {
                    imageDescriptors[1] = material->metallicRoughnessTexture->descriptor;
                }
            }

            if (material->pbrWorkflows.specularGlossiness) {
                if (material->extension.diffuseTexture) {
                    imageDescriptors[0] = material->extension.diffuseTexture->descriptor;
                }
                if (material->extension.specularGlossinessTexture) {
                    imageDescriptors[1] = material->extension.specularGlossinessTexture->descriptor;
                }
            }

            std::array<VkWriteDescriptorSet, 5> writeDescriptorSets{};
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

    std::shared_ptr<Model> AssetsManager::getModel(uint32_t name) {
        return models[name];
    }

    std::shared_ptr<Mesh> AssetsManager::getMesh(uint32_t name) {
        return meshes[name];
    }

    std::shared_ptr<Texture> AssetsManager::getTexture(uint32_t id) {
        return textures[id];
    }

    VkDescriptorSetLayout AssetsManager::getDescriptorSetLayout() {
        return descriptorSetLayout;
    }

} // namespace re