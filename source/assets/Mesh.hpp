#ifndef RAVENENGINE_MESH_HPP
#define RAVENENGINE_MESH_HPP


#include <memory>
#include <vector>

#include "vk_mem_alloc.h"

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "math/Vector4.hpp"
#include "utils/NonCopyable.hpp"


namespace re {

    class Device;
    class Buffer;

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
            vec3 color{};
            vec3 normal{};
            vec2 uv0{};
            vec2 uv1{};
            vec4 joint0{};
            vec4 weight0{};

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
         */
        Mesh(std::shared_ptr<Device> device, const Data& data);

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


#endif //RAVENENGINE_MESH_HPP
