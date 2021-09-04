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

    struct PushConstant {
        mat4 mvp{1.f};
        mat4 node{1.f};
    };

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName,
                     std::shared_ptr<AssetsManager> assetsManager);

        ~RenderSystem() override;

        void renderScene(VkCommandBuffer commandBuffer);

        std::shared_ptr<Entity> getCamera();

        /**
         *
         * @param aspect SwapChain extent aspect ratio
         */
        void update(float aspect);

        void setScene(std::shared_ptr<Scene> scene);

    private:
        std::shared_ptr<Scene> scene;
        std::shared_ptr<AssetsManager> assetsManager;
        std::shared_ptr<Entity> camera;
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
