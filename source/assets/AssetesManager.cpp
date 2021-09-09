#include "AssetsManager.hpp"

#include "spdlog/spdlog.h"

#include "Model.hpp"
#include "Texture.hpp"
#include "render/Device.hpp"
#include "files/FilesManager.hpp"
#include "files/File.hpp"
#include "utils/Macros.hpp"
#include "render/Buffer.hpp"


namespace re {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {
        createDescriptorPool();
        createDescriptorSetLayout();
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

        // TODO: Implement materials
        const tinygltf::Material material = model.materials[mesh.primitives[0].material];
        if (material.pbrMetallicRoughness.baseColorTexture.index > -1) {
            const tinygltf::Texture texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];

            return meshes[meshID] = std::make_shared<Mesh>(device, data, addTexture(model, texture));
        }
    }

    std::shared_ptr<Texture> AssetsManager::addTexture(const tinygltf::Model& gltfModel, const tinygltf::Texture &gltfTexture) {
        const tinygltf::Image image = gltfModel.images[gltfTexture.source];
        uint32_t textureID = std::hash<std::string>()(image.name);

        if (textures.find(textureID) != textures.end()) return textures[textureID];

        int width, height;
        VkDeviceSize imageSize;
        stbi_uc* pixels = loadImageFile(FilesManager::getFile(("textures/" + image.uri).c_str()).getPath(), &width, &height, &imageSize);

        Buffer stagingBuffer(device->getAllocator(), imageSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.copyTo(pixels);
        stagingBuffer.unmap();

        stbi_image_free(pixels);

        VkExtent2D textureSize = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        auto mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height))));

        VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent = {textureSize.width, textureSize.height, 1};
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1,
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        Texture::Sampler sampler{};
        if (gltfTexture.sampler > -1) {
            tinygltf::Sampler smpl = gltfModel.samplers[gltfTexture.sampler];
            sampler.minFilter = Texture::Sampler::getVkFilterMode(smpl.minFilter);
            sampler.magFilter = Texture::Sampler::getVkFilterMode(smpl.magFilter);
            sampler.addressModeU = Texture::Sampler::getVkWrapMode(smpl.wrapS);
            sampler.addressModeV = Texture::Sampler::getVkWrapMode(smpl.wrapT);
            sampler.addressModeW = sampler.addressModeV;
        } else {
            sampler.magFilter = VK_FILTER_LINEAR;
            sampler.minFilter = VK_FILTER_LINEAR;
            sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }

        Texture::Info info{imageInfo, VMA_MEMORY_USAGE_CPU_COPY, VK_IMAGE_ASPECT_COLOR_BIT};
        textures[textureID] = std::make_shared<Texture>(device, info, sampler);
        auto texture = textures[textureID];

        device->transitionImageLayout(texture->getImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
        device->copyBufferToImage(stagingBuffer, *texture);

        texture->generateMipmaps(device);
        texture->createDescriptorSet(descriptorPool, descriptorSetLayout);

        return texture;
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

    stbi_uc * AssetsManager::loadImageFile(const std::string& fileName, int* width, int* height, VkDeviceSize* size) {
        int channels;

        stbi_uc* image = stbi_load(fileName.c_str(), width, height, &channels, STBI_rgb_alpha);

        if (!image) RE_THROW_EX("Failed to image file: " + fileName);

        *size = *width * *height * static_cast<int>(STBI_rgb_alpha);

        return image;
    }

    void AssetsManager::createDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = MAX_OBJECTS;

        VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        poolInfo.maxSets = MAX_OBJECTS;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;

        vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool);
    }

    void AssetsManager::createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layoutBinding.pImmutableSamplers = VK_NULL_HANDLE;

        VkDescriptorSetLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;

        vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout);
    }

} // namespace re