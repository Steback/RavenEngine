#include "AssetsManager.hpp"

#include "Model.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "engine/render/Device.hpp"
#include "engine/utils/Utils.hpp"
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

    /**
     * @brief Create the Descriptor pool used by all needed assists
     * @param imageCount SwapChain image count
     */
    void AssetsManager::setupDescriptorsPool(uint32_t imageCount) {
        // TODO: Find the way to do this dynamically
        uint32_t uboCount = 2;
        uint32_t standaloneTexCount = 1;
        uint32_t imageSamplerCount = 0;
        uint32_t materialCount = 0;

        for (auto& asset : assets) {
            if (asset.second->type == Asset::MATERIAL) {
                imageSamplerCount += 1;
                materialCount++;
            }
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

    /**
     * @brief Allocate Vulkan Descriptor set
     * @param type Type of Descriptor Set. Define the layout used to create it.
     * @param set Pointer to set to allocate.
     */
    void AssetsManager::allocateDescriptorSet(DescriptorSetType type, VkDescriptorSet *set) {
        VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &layouts[type];
        vkAllocateDescriptorSets(device->getDevice(), &allocateInfo, set);
    }

    /**
     *
     * @param type Type of descriptor set layout required
     * @return Vulkan Descriptor set layout
     */
    VkDescriptorSetLayout AssetsManager::getDescriptorSetLayout(DescriptorSetType type) {
        return layouts[type];
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
                {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
                {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}
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
        for (auto& [id, asset] : assets) {
            if (asset->type == Asset::MATERIAL) {
                auto* material = reinterpret_cast<Material*>(asset);
                VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
                descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                descriptorSetAllocInfo.descriptorPool = descriptorPool;
                descriptorSetAllocInfo.pSetLayouts = &layouts[MATERIAL];
                descriptorSetAllocInfo.descriptorSetCount = 1;

                checkResult(vkAllocateDescriptorSets(device->getDevice(), &descriptorSetAllocInfo, &material->descriptorSet),
                            "Failed to allocate material descriptor sets");

                VkDescriptorImageInfo empty = get<Texture>(std::hash<std::string>()("empty"))->descriptor;
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
    }

} // namespace re