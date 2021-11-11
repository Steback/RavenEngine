#include "AssetsManager.hpp"

#include "Model.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "engine/render/Device.hpp"
#include "engine/core/Utils.hpp"
#include "engine/render/buffers/Buffer.hpp"
#include "engine/scene/Skybox.hpp"


namespace re {

    AssetsManager* AssetsManager::singleton;

    // TODO: Keep raw pointer to Assets?, find a way to use Smart pointers.
    /**
     *
     * @param device Pointer to Device object
     */
    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {
        add<Texture>("empty", this->device, "empty.png", Texture::Sampler{});
    }

    AssetsManager::~AssetsManager() {
        for (auto& [type, layout] : layouts)
            vkDestroyDescriptorSetLayout(device->getDevice(), layout, nullptr);

        vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);

        for (auto& [id, asset] : assets)
            delete asset;
    }

    /**
     *
     * @return Instance of AssetsManager singleton
     */
    AssetsManager* AssetsManager::getInstance() {
        return singleton;
    }

    // TODO: Change how to create a Skybox
    /**
     * @brief Load Skybox form a .ktx file
     * @param name Name of file
     * @param renderPass Valid Vulkan render pass
     * @return Pointer to Skybox
     */
    std::unique_ptr<Skybox> AssetsManager::loadSkybox(const std::string &name, VkRenderPass renderPass) {
        auto* model = add<Model>("SkyboxMesh", "models/cube.gltf");
        auto* texture = add<Texture>("SkyboxTexture", device, name, Texture::Sampler{}, true);

        return std::make_unique<Skybox>(device, renderPass, model, texture);
    }

    // TODO: Should this will removed?
    std::shared_ptr<Device> AssetsManager::getDevice() {
        return device;
    }

} // namespace re