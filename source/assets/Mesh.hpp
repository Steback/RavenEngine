#ifndef LITTLEVULKANENGINE_MESH_HPP
#define LITTLEVULKANENGINE_MESH_HPP


#include <memory>
#include <vector>

#include "vk_mem_alloc.h"

#include "math/Vector3.hpp"
#include "math/Vector2.hpp"
#include "utils/NonCopyable.hpp"


namespace re {

    class Device;
    class Buffer;

    class Mesh : NonCopyable {
    public:
        struct Vertex {
            vec3 position;
            vec3 color;
            vec3 normal;
            vec2 uv;

            bool operator==(const Vertex &other) const;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct Data {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };

    public:
        Mesh(std::shared_ptr<Device> device, const Data& data);

        ~Mesh() override;

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer) const;

    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);

        void createIndexBuffer(const std::vector<uint32_t>& indices);

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<Buffer> vertexBuffer;
        std::unique_ptr<Buffer> indexBuffer;
        uint32_t vertexCount{};
        uint32_t indexCount{};
        bool hasIndexBuffer{false};
    };

} // namespace re


#endif //LITTLEVULKANENGINE_MESH_HPP
