#ifndef RAVENENGINE_MESH_HPP
#define RAVENENGINE_MESH_HPP


#include <memory>
#include <vector>

#include "vk_mem_alloc.h"
#include "tiny_gltf.h"

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "math/Vector4.hpp"
#include "utils/NonCopyable.hpp"


namespace re {

    class Device;
    class Buffer;
    class Texture;

    /**
     * @brief Mesh Class
     */
    class Mesh : NonCopyable {
    public:
        /**
         * @brief Vertex class
         */
        struct Vertex {
            vec3 position{};
            vec3 normal{};
            vec2 uv{};

            /**
             *
             * @return Vector with the Vulkan Vertex Input Binding Description of Vertex
             */
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            /**
             *
             * @return Vector with the Vulkan Input Attributes Description of Vertex
             */
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const;
        };

        /**
         * @brief Vertex create Data. Vertices and indices
         */
        struct Data {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };

    public:
        /**
         * Default constructor
         * @param device shared_ptr of Device
         * @param data Data object
         * @param texture Valid pointer to texture. By default is null
         */
        Mesh(std::shared_ptr<Device> device, const Data& data, std::shared_ptr<Texture> texture = nullptr);

        ~Mesh() override;

        /**
         * @brief Bind mesh
         * @param commandBuffer Command buffer in recording state
         */
        void bind(VkCommandBuffer commandBuffer);

        /**
         * @brief Draw mesh
         * @param commandBuffer Command buffer in recording state
         */
        void draw(VkCommandBuffer commandBuffer) const;

        [[nodiscard]] uint32_t getVertexCount() const;

        [[nodiscard]] uint32_t getIndexCount() const;

        [[nodiscard]] bool isHasIndexBuffer() const;

        [[nodiscard]] std::shared_ptr<Texture> getTexture() const;

        /**
         * @brief Load Mesh from GLTF2 file
         * @param model TinyGLTF Model
         * @param mesh  TinyGLTF mesh
         * @return Mesh vertices and Indices
         */
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
        std::shared_ptr<Texture> texture;
    };

} // namespace re


#endif //RAVENENGINE_MESH_HPP
