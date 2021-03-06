#ifndef RAVENENGINE_RENDERSYSTEM_HPP
#define RAVENENGINE_RENDERSYSTEM_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "engine/core/NonCopyable.hpp"
#include "engine/entity/components/Camera.hpp"
#include "engine/entity/components/Transform.hpp"
#include "engine/entity/components/MeshRender.hpp"
#include "engine/entity/components/Light.hpp"


namespace re {

    class Device;
    class GraphicsPipeline;
    class Scene;
    class Entity;
    class AssetsManager;
    class UniformBuffer;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName);

        ~RenderSystem() override;

        void renderScene(VkCommandBuffer commandBuffer, const std::shared_ptr<Scene>& scene);

        std::shared_ptr<Entity> getCamera();

        void update(float aspect);

    private:
        // TODO: Remove this. Temporally uboBuffer.
        void setupBuffer();

        void updateBuffer();

    private:
        std::shared_ptr<Entity> camera;
        std::shared_ptr<Entity> light;
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
        VkDescriptorSet uboDescriptorSet{};
        // TODO: Change this
        std::unique_ptr<UniformBuffer> uboTransformBuffer;
        std::unique_ptr<UniformBuffer> uboNodeBuffer;
        std::unique_ptr<UniformBuffer> uboLightBuffer;
        Transform::Ubo uboTransform{};
        Model::Ubo uboNode{};
        Light::Ubo uboLight;
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
