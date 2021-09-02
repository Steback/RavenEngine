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

    }

    AssetsManager::~AssetsManager() = default;

    std::shared_ptr<Model> AssetsManager::loadModel(const std::string &fileName) {
        File file = FilesManager::getFile(fileName.c_str());
        uint32_t nameHash = std::hash<std::string>()(file.getName(true));

        if (models.find(nameHash) != models.end()) return models[nameHash];

        std::string error;
        std::string warning;

        if (file.getExtension() == ".obj") {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, file.getPath().c_str()))
                RE_THROW_EX(warning + error);

            return models[nameHash] = std::make_shared<Model>(this, fileName, attrib, shapes);
        } else {
            tinygltf::Model gltfModel;
            tinygltf::TinyGLTF gltfContext;

            bool fileLoaded;
            if (file.getExtension() == ".glb")
                fileLoaded = gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, file.getPath());
            else
                fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, file.getPath());

            if (fileLoaded)
                return models[nameHash] = std::make_shared<Model>(this, fileName, gltfModel);
        }

        return nullptr;
    }

    std::shared_ptr<Mesh> AssetsManager::addMesh(const std::string &name, const Mesh::Data& data) {
        return meshes[std::hash<std::string>()(name)] = std::make_shared<Mesh>(device, data);
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

        return meshes[meshID] = std::make_shared<Mesh>(device, data);
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
        textures[textureID] = std::make_shared<Texture>(device->getDevice(), device->getAllocator(), imageInfo, VMA_MEMORY_USAGE_CPU_COPY, VK_IMAGE_ASPECT_COLOR_BIT);
        auto texture = textures[textureID];

        device->transitionImageLayout(texture->getImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
        device->copyBufferToImage(stagingBuffer, *texture);

        texture->generateMipmaps(device);
        // TODO: Create descriptor pool and layout then create texture set
//        texture->createDescriptorSet();

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

    stbi_uc * AssetsManager::loadImageFile(const std::string& fileName, int* width, int* height, VkDeviceSize* size) {
        int channels;

        stbi_uc* image = stbi_load(fileName.c_str(), width, height, &channels, STBI_rgb_alpha);

        if (!image) RE_THROW_EX("Failed to image file: " + fileName);

        *size = *width * *height * static_cast<int>(STBI_rgb_alpha);

        return image;
    }

} // namespace re