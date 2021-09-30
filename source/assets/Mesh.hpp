#ifndef RAVENENGINE_MESH_HPP
#define RAVENENGINE_MESH_HPP


#include <memory>
#include <vector>

#include "vk_mem_alloc.h"
#include "tiny_gltf.h"

#include "Asset.hpp"
#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "math/Vector4.hpp"


namespace re {

    class Device;
    class Buffer;
    class Material;

    class Mesh : public Asset {
        friend class Model;

    public:
        struct Vertex {
            vec3 position{};
            vec3 normal{};
            vec2 uv0{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const;
        };

        struct Data {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };

        Mesh(std::string name, std::shared_ptr<Device> device, const tinygltf::Model &model, const tinygltf::Node &node);

        ~Mesh() override;

        void bind(VkCommandBuffer commandBuffer) const;

        void draw(VkCommandBuffer commandBuffer) const;

        [[nodiscard]] uint32_t getVertexCount() const;

        [[nodiscard]] uint32_t getIndexCount() const;

        [[nodiscard]] bool isHasIndexBuffer() const;

        [[nodiscard]] const Material* getMaterial() const;

        static Data loadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);

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
        Material* material;
    };

} // namespace re


#endif //RAVENENGINE_MESH_HPP
