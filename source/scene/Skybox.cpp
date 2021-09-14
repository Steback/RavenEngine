#include "Skybox.hpp"

#include "assets/Mesh.hpp"
#include "assets/Texture.hpp"
#include "render/Device.hpp"
#include "render/Buffer.hpp"
#include "render/pipelines/GraphicsPipeline.hpp"


namespace re {

    Skybox::Skybox(std::shared_ptr<Device> device, VkRenderPass renderPass, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture)
            : device(std::move(device)), mesh(std::move(mesh)), texture(std::move(texture)) {
        setupBuffer();
        setupDescriptors();

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                "skybox.vert", "skybox.frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{descriptorSetLayout},
                std::vector<VkPushConstantRange>{}
        );
    }

    Skybox::~Skybox() {
        uboBuffer->unmap();
        vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device->getDevice(), descriptorSetLayout, nullptr);
    }

    void Skybox::draw(VkCommandBuffer commandBuffer, const mat4& proj, const mat4& transform) {
        pipeline->bind(commandBuffer);

        std::vector<VkDescriptorSet> sets = {descriptorSet};
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
    void Skybox::setupDescriptors() {
        std::vector<VkDescriptorPoolSize> poolSizes = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}
        };

        VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 1;

        vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool);

        std::vector<VkDescriptorSetLayoutBinding> bindings = {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}
        };

        VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        descriptorLayoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        descriptorLayoutInfo.pBindings = bindings.data();

        vkCreateDescriptorSetLayout(device->getDevice(), &descriptorLayoutInfo, nullptr, &descriptorSetLayout);

        VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &descriptorSetLayout;
        vkAllocateDescriptorSets(device->getDevice(), &allocateInfo, &descriptorSet);

        std::vector<VkWriteDescriptorSet> writeDescriptors(2);
        writeDescriptors[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptors[0].descriptorCount = 1;
        writeDescriptors[0].dstSet = descriptorSet;
        writeDescriptors[0].dstBinding = 0;
        writeDescriptors[0].pBufferInfo = &uboBuffer->descriptor;

        writeDescriptors[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptors[1].descriptorCount = 1;
        writeDescriptors[1].dstSet = descriptorSet;
        writeDescriptors[1].dstBinding = 1;
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
