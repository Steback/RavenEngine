#ifndef RAVENENGINE_RENDERSYSTEM_HPP
#define RAVENENGINE_RENDERSYSTEM_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "entity/components/Camera.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshRender.hpp"


namespace re {

    class Device;
    class GraphicsPipeline;
    class Scene;
    class Entity;
    class AssetsManager;
    class Buffer;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName);

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

        // TODO: Remove this. Temporally uboBuffer.
        void setupBuffer();

        void updateBuffer();

    private:
        std::shared_ptr<Entity> camera;
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
        VkDescriptorSet uboDescriptorSet{};
        std::unique_ptr<Buffer> uboTransformBuffer;
        std::unique_ptr<Buffer> uboNodeBuffer;
        Transform::Ubo uboTransform{};
        Model::Ubo uboNode{};
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
