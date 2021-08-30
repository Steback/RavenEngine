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

    struct MvpPushConstant {
        mat4 mvp{1.f};
    };

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName, Scene& scene);

        ~RenderSystem() override;

        void renderScene(VkCommandBuffer commandBuffer);

        /**
         *
         * @param aspect SwapChain extent aspect ratio
         */
        void setProjection(float aspect);

        std::shared_ptr<Entity> getCamera();

    private:
        Scene& scene;
        std::shared_ptr<Entity> camera;
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
