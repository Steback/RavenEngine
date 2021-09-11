#ifndef RAVENENGINE_RENDERSYSTEM_HPP
#define RAVENENGINE_RENDERSYSTEM_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "entity/components/Camera.hpp"


namespace re {

    class Device;
    class GraphicsPipeline;
    class Scene;
    class Entity;
    class AssetsManager;
    class Buffer;

    // TODO: Change this for a dynamic uniform object
    struct UboModel {
        mat4 mvp{1.f};
        mat4 node{1.f};
    };

    struct PushConstBlockMaterial {
        vec4 baseColorFactor;
        vec4 emissiveFactor;
        vec4 diffuseFactor;
        vec4 specularFactor;
        float workflow;
        int colorTextureSet;
        int PhysicalDescriptorTextureSet;
        int normalTextureSet;
        int occlusionTextureSet;
        int emissiveTextureSet;
        float metallicFactor;
        float roughnessFactor;
        float alphaMask;
        float alphaMaskCutoff;
    };

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName,
                     std::shared_ptr<AssetsManager> assetsManager);

        ~RenderSystem() override;

        void renderScene(VkCommandBuffer commandBuffer, const std::shared_ptr<Scene>& scene);

        std::shared_ptr<Entity> getCamera();

        /**
         *
         * @param aspect SwapChain extent aspect ratio
         */
        void update(float aspect);

    private:
        void setupDescriptors();

        // TODO: Remove this. Temporally buffer.
        void setupBuffer();

        void updateBuffer();

    private:
        std::shared_ptr<AssetsManager> assetsManager;
        std::shared_ptr<Entity> camera;
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
        VkDescriptorPool descriptorPool{};
        VkDescriptorSetLayout descriptorSetLayout{};
        VkDescriptorSet descriptorSet{};
        std::unique_ptr<Buffer> uboModelBuffer;
        UboModel uboModel;
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
