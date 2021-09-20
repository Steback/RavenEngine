#ifndef RAVENENGINE_SKYBOX_HPP
#define RAVENENGINE_SKYBOX_HPP


#include <memory>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "math/Matrix4.hpp"


namespace re {

    class GraphicsPipeline;
    class Mesh;
    class Texture;
    class UniformBuffer;
    class Device;
    class AssetsManager;

    /**
     * @brief Skybox wrapper class
     */
    class Skybox : NonCopyable {
    public:
        struct UboData {
            mat4 projection;
            mat4 transform;
        };

    public:
        Skybox(std::shared_ptr<Device> device, VkRenderPass renderPass);

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
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Texture> texture;
        std::unique_ptr<UniformBuffer> uboBuffer;
        UboData uboData{};
    };

} // namespace re


#endif //RAVENENGINE_SKYBOX_HPP
