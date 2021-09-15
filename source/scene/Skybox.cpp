#include "Skybox.hpp"

#include "assets/Model.hpp"
#include "assets/Texture.hpp"
#include "assets/AssetsManager.hpp"
#include "render/Device.hpp"
#include "render/Buffer.hpp"
#include "render/pipelines/GraphicsPipeline.hpp"


namespace re {

    Skybox::Skybox(std::shared_ptr<Device> device, VkRenderPass renderPass, AssetsManager* assetsManager)
            : device(std::move(device))  {
        uint32_t skyboxHash = std::hash<std::string>()("Skybox");
        mesh = assetsManager->getModel(skyboxHash)->getNode(0).mesh;
        texture = assetsManager->getTexture(skyboxHash);

        setupBuffer();
        setupDescriptors(assetsManager);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                "skybox.vert", "skybox.frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{assetsManager->getUboLayout(), assetsManager->getTextureLayout()},
                std::vector<VkPushConstantRange>{}
        );
    }

    Skybox::~Skybox() {
        uboBuffer->unmap();
    }

    void Skybox::draw(VkCommandBuffer commandBuffer, const mat4& proj, const mat4& transform) {
        pipeline->bind(commandBuffer);

        std::vector<VkDescriptorSet> sets = {uboDescriptorSet, textureDescriptorSet};
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->getLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                0, nullptr);

        uboData.projection = proj;
        uboData.transform = transform;
        uboBuffer->copyTo(&uboData);

        mesh->bind(commandBuffer);
        mesh->draw(commandBuffer);
    }

    // TODO: Remove this descriptor pool and layout from here
    void Skybox::setupDescriptors(AssetsManager* assetsManager) {
        assetsManager->allocateDescriptorSet(DescriptorSetType::UBO, &uboDescriptorSet);
        assetsManager->allocateDescriptorSet(DescriptorSetType::TEXTURE, &textureDescriptorSet);

        std::vector<VkWriteDescriptorSet> writeDescriptors(2);
        writeDescriptors[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptors[0].descriptorCount = 1;
        writeDescriptors[0].dstSet = uboDescriptorSet;
        writeDescriptors[0].dstBinding = 0;
        writeDescriptors[0].pBufferInfo = &uboBuffer->descriptor;

        writeDescriptors[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptors[1].descriptorCount = 1;
        writeDescriptors[1].dstSet = textureDescriptorSet;
        writeDescriptors[1].dstBinding = 0;
        writeDescriptors[1].pImageInfo = &texture->descriptor;
        vkUpdateDescriptorSets(device->getDevice(), static_cast<uint32_t>(writeDescriptors.size()), writeDescriptors.data(), 0, nullptr);
    }

    void Skybox::setupBuffer() {
        uboBuffer = std::make_unique<Buffer>(
                device->getAllocator(),
                sizeof(UboData),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_MEMORY_USAGE_CPU_TO_GPU
        );
        uboBuffer->map();
    }

} // namespace re
