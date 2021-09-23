#include "Skybox.hpp"

#include "assets/Model.hpp"
#include "assets/Texture.hpp"
#include "assets/AssetsManager.hpp"
#include "render/Device.hpp"
#include "render/buffers/UniformBuffer.hpp"
#include "render/pipelines/GraphicsPipeline.hpp"


namespace re {

    // TODO: Refactored Skybox class and Add doxygen comments
    Skybox::Skybox(std::shared_ptr<Device> device, VkRenderPass renderPass) : device(std::move(device))  {
        uint32_t skyboxHash = std::hash<std::string>()("Skybox");
        mesh = AssetsManager::getInstance()->getModel(skyboxHash)->getNode(0).mesh;
        texture = AssetsManager::getInstance()->getTexture(skyboxHash);

        setupBuffer();
        setupDescriptors();

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                "skybox.vert", "skybox.frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{AssetsManager::getInstance()->getDescriptorSetLayout(UBO), AssetsManager::getInstance()->getDescriptorSetLayout(TEXTURE)},
                std::vector<VkPushConstantRange>{}
        );
    }

    Skybox::~Skybox() = default;

    void Skybox::draw(VkCommandBuffer commandBuffer, const mat4& proj, const mat4& transform) {
        pipeline->bind(commandBuffer);

        std::vector<VkDescriptorSet> sets = {uboDescriptorSet, textureDescriptorSet};
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->getLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                0, nullptr);

        uboData.projection = proj;
        uboData.transform = transform;
        uboBuffer->update(&uboData);

        mesh->bind(commandBuffer);
        mesh->draw(commandBuffer);
    }

    // TODO: Remove this descriptor pool and layout from here
    void Skybox::setupDescriptors() {
        AssetsManager::getInstance()->allocateDescriptorSet(DescriptorSetType::UBO, &uboDescriptorSet);
        AssetsManager::getInstance()->allocateDescriptorSet(DescriptorSetType::TEXTURE, &textureDescriptorSet);

        std::vector<VkWriteDescriptorSet> writeDescriptors(2);
        writeDescriptors[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptors[0].descriptorCount = 1;
        writeDescriptors[0].dstSet = uboDescriptorSet;
        writeDescriptors[0].dstBinding = 0;
        VkDescriptorBufferInfo descriptor = uboBuffer->descriptorInfo();
        writeDescriptors[0].pBufferInfo = &descriptor;

        writeDescriptors[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptors[1].descriptorCount = 1;
        writeDescriptors[1].dstSet = textureDescriptorSet;
        writeDescriptors[1].dstBinding = 0;
        writeDescriptors[1].pImageInfo = &texture->descriptor;
        vkUpdateDescriptorSets(device->getDevice(), static_cast<uint32_t>(writeDescriptors.size()), writeDescriptors.data(), 0, nullptr);
    }

    void Skybox::setupBuffer() {
        uboBuffer = std::make_unique<UniformBuffer>(
                device->getAllocator(),
                sizeof(UboData)
        );
    }

} // namespace re
