#ifndef RAVENENGINE_SKYBOX_HPP
#define RAVENENGINE_SKYBOX_HPP


#include <memory>

#include "vulkan/vulkan.h"

#include "engine/utils/NonCopyable.hpp"
#include "engine/math/Matrix4.hpp"


namespace re {

    class GraphicsPipeline;
    class Model;
    class Mesh;
    class Texture;
    class UniformBuffer;
    class Device;
    class AssetsManager;

    class Skybox : NonCopyable {
    public:
        struct UboData {
            mat4 projection;
            mat4 transform;
        };

    public:
        Skybox(std::shared_ptr<Device> device, VkRenderPass renderPass, Model* model_, Texture* texture_);

        ~Skybox() override;

        void draw(VkCommandBuffer commandBuffer, const mat4& proj, const mat4& transform);

    private:
        void setupDescriptors();

        void setupBuffer();

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
        VkDescriptorSet uboDescriptorSet{};
        VkDescriptorSet textureDescriptorSet{};
        const Mesh* mesh;
        const Texture* texture;
        std::unique_ptr<UniformBuffer> uboBuffer;
        UboData uboData{};
    };

} // namespace re


#endif //RAVENENGINE_SKYBOX_HPP
