#include "Skybox.hpp"

#include "engine/assets/Model.hpp"
#include "engine/assets/Texture.hpp"
#include "engine/assets/AssetsManager.hpp"
#include "engine/render/Device.hpp"
#include "engine/render/buffers/UniformBuffer.hpp"
#include "engine/render/pipelines/GraphicsPipeline.hpp"


namespace re {

    // TODO: Refactored Skybox class and Add doxygen comments
    Skybox::Skybox(std::shared_ptr<Device> device, VkRenderPass renderPass, Model* model_, Texture* texture_) : device(std::move(device))  {
        mesh = model_->getNode(0).mesh;
        texture = texture_;

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
        uboBuffer->writeTo(&uboData);

        // TODO: Temporally unused
//        mesh->bind(commandBuffer);
//        mesh->draw(commandBuffer);
    }

    void Skybox::setupBuffer() {
        uboBuffer = std::make_unique<UniformBuffer>(
                device->getAllocator(),
                sizeof(UboData)
        );
    }

} // namespace re
