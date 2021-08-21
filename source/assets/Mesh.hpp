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

    /**
     * @brief Mesh Class
     */
    class Mesh : NonCopyable {
    public:
        /**
         * @brief Vertex class
         */
        struct Vertex {
            vec3 position;
            vec3 color;
            vec3 normal;
            vec2 uv;

            bool operator==(const Vertex &other) const;

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

    private:
        /**
         * @brief Create vertex buffer in GPU only use
         * @param vertices vector of Vertex
         */
        void createVertexBuffer(const std::vector<Vertex>& vertices);

        /**
         * @brief Create index buffer in GPU only use
         * @param indices vector of unsigned int
         */
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
