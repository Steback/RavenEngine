#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "scene/Skybox.hpp"
#include "math/Common.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/Entity.hpp"
#include "render/Buffer.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


namespace re {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName, std::shared_ptr<AssetsManager> assetsManager)
            : device(std::move(device)), assetsManager(std::move(assetsManager)) {
        VkPushConstantRange materialPushConstant{};
        materialPushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        materialPushConstant.size = sizeof(PushConstBlockMaterial);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);

        setupBuffer();
        setupDescriptors();

        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert", shadersName + ".frag",
                configInfo,
                std::vector<VkDescriptorSetLayout>{descriptorSetLayout, this->assetsManager->getDescriptorSetLayout()},
                std::vector<VkPushConstantRange>{materialPushConstant}
        );
    }

    RenderSystem::~RenderSystem() {
        uboModelBuffer->unmap();
        vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device->getDevice(), descriptorSetLayout, nullptr);
    }

    void RenderSystem::renderScene(VkCommandBuffer commandBuffer, const std::shared_ptr<Scene>& scene) {
        if (!camera) camera = scene->getEntity("Camera");

        auto& cameraComponent = camera->getComponent<Camera>();
        mat4 viewProj = cameraComponent.getProjection() * cameraComponent.getView();

        scene->skybox->draw(commandBuffer, cameraComponent.getProjection(), Matrix4{1.0f});

        pipeline->bind(commandBuffer);

        std::vector<VkDescriptorSet> sets = {descriptorSet};
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->getLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                0, nullptr);

        for (auto& id : scene->getRegistry().view<Transform, MeshRender>()) {
            auto entity = scene->getEntity(id);
            auto& transform = entity->getComponent<Transform>();
            auto& meshRender = entity->getComponent<MeshRender>();

            uboModel.mvp = viewProj * transform.getWorldMatrix();

            meshRender.model->render(commandBuffer, pipeline->getLayout(), uboModel);
            updateBuffer();
        }
    }

    std::shared_ptr<Entity> RenderSystem::getCamera() {
        return camera;
    }

    void RenderSystem::update(float aspect) {
        if (camera) {
            auto& cameraComponent = camera->getComponent<Camera>();
            cameraComponent.setPerspectiveProjection(radians(50.f), aspect, 0.1f, 10.f);
        }
    }

    // TODO: Remove this descriptor pool and layout from here
    void RenderSystem::setupDescriptors() {
        std::vector<VkDescriptorPoolSize> poolSizes = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}
        };

        VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 1;

        vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool);

        std::vector<VkDescriptorSetLayoutBinding> bindings = {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT}
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

        VkWriteDescriptorSet writeDescriptorSet{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.dstSet = descriptorSet;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.pBufferInfo = &uboModelBuffer->descriptor;
        vkUpdateDescriptorSets(device->getDevice(), 1, &writeDescriptorSet, 0, nullptr);
    }

    void RenderSystem::setupBuffer() {
        uboModelBuffer = std::make_unique<Buffer>(
                device->getAllocator(),
                sizeof(UboModel),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_MEMORY_USAGE_CPU_TO_GPU
        );
        uboModelBuffer->map();
    }

    // TODO: Remove this
    void RenderSystem::updateBuffer() {
        uboModelBuffer->copyTo(&uboModel);
    }

} // namespace re