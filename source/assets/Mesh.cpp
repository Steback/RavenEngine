#include "Mesh.hpp"

#include "utils/Macros.hpp"
#include "render/Device.hpp"
#include "render/Buffer.hpp"


namespace re {

    std::vector<VkVertexInputBindingDescription> Mesh::Vertex::getBindingDescriptions() {
        return {
            { 0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
        };
    }

    std::vector<VkVertexInputAttributeDescription> Mesh::Vertex::getAttributeDescriptions() {
        return {
            { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) },
            { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) },
            { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) },
            { 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) },
        };
    }

    bool Mesh::Vertex::operator==(const Mesh::Vertex &other) const {
        return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
    }

    Mesh::Mesh(std::shared_ptr<Device> device, const Data& data) : device(std::move(device)) {
        createVertexBuffer(data.vertices);
        createIndexBuffer(data.indices);
    }

    Mesh::~Mesh() = default;

    void Mesh::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void Mesh::draw(VkCommandBuffer commandBuffer) const {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void Mesh::createVertexBuffer(const std::vector<Vertex> &vertices) {
        vertexCount = CAST_U32(vertices.size());
        VkDeviceSize size = sizeof(vertices[0]) * vertexCount;

        Buffer stagingBuffer(device->getAllocator(), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.copyTo(vertices.data());
        stagingBuffer.unmap();

        vertexBuffer = std::make_unique<Buffer>(device->getAllocator(), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        device->copyBuffer(stagingBuffer, *vertexBuffer, size);
    }

    void Mesh::createIndexBuffer(const std::vector<uint32_t>& indices) {
        indexCount = CAST_U32(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) return;

        VkDeviceSize size = sizeof(indices[0]) * indexCount;

        Buffer stagingBuffer(device->getAllocator(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.copyTo(indices.data());
        stagingBuffer.unmap();

        indexBuffer = std::make_unique<Buffer>(device->getAllocator(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        device->copyBuffer(stagingBuffer, *indexBuffer, size);
    }

} // namespace lv