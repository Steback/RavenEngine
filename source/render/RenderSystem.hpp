#ifndef RAVENENGINE_RENDERSYSTEM_HPP
#define RAVENENGINE_RENDERSYSTEM_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "Camera.hpp"


namespace re {

    class Device;
    class GraphicsPipeline;
    class Scene;
    class Entity;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName, Scene& scene);

        ~RenderSystem() override;

        void renderScene(VkCommandBuffer commandBuffer);

        /**
         *
         * @param type Camera::Type view. By default is DIRECTION
         */
        void setView(Camera::Type type = Camera::Type::DIRECTION);

        /**
         *
         * @param aspect SwapChain extent aspect ratio
         */
        void setProjection(float aspect);

    private:
        Scene& scene;
        Camera camera;
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
        std::shared_ptr<Entity> cameraEntity;
    };

} // namespace re


#endif //RAVENENGINE_RENDERSYSTEM_HPP
