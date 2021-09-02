#include "Mesh.hpp"

#include "spdlog/spdlog.h"

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
            { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) },
            { 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv0) },
            { 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv1) },
            { 4, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, joint0) },
            { 5, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, weight0) }
        };
    }

    bool Mesh::Vertex::operator==(const Mesh::Vertex &other) const {
        return position == other.position && normal == other.normal && uv0 == other.uv0;
    }

    Mesh::Mesh(std::shared_ptr<Device> device, const Data& data, std::shared_ptr<Texture> texture)
            : device(std::move(device)), texture(std::move(texture)) {
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

    uint32_t Mesh::getVertexCount() const {
        return vertexCount;
    }

    uint32_t Mesh::getIndexCount() const {
        return indexCount;
    }

    bool Mesh::isHasIndexBuffer() const {
        return hasIndexBuffer;
    }

    Mesh::Data Mesh::loadMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh) {
        std::vector<Mesh::Vertex> vertices;
        std::vector<uint32_t> indices;

        for (auto primitive : mesh.primitives) {
            uint32_t indexCount = 0;
            uint32_t vertexCount = 0;

            // Vertices
            {
                const float *bufferPos = nullptr;
                const float *bufferNormals = nullptr;
                const float *bufferTexCoordSet0 = nullptr;
                const float *bufferTexCoordSet1 = nullptr;
                const void *bufferJoints = nullptr;
                const float *bufferWeights = nullptr;

                int posByteStride;
                int normByteStride;
                int uv0ByteStride;
                int uv1ByteStride;
                int jointByteStride;
                int weightByteStride;

                int jointComponentType;

                if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("POSITION")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferPos = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    vertexCount = static_cast<uint32_t>(accessor.count);
                    posByteStride = accessor.ByteStride(view) ? (accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);
                }

                if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("NORMAL")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferNormals = reinterpret_cast<const float *>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    normByteStride = accessor.ByteStride(view) ? (accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);
                }

                if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferTexCoordSet0 = reinterpret_cast<const float *>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    uv0ByteStride = accessor.ByteStride(view) ? (accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2);
                }

                if (primitive.attributes.find("TEXCOORD_1") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("TEXCOORD_1")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferTexCoordSet1 = reinterpret_cast<const float *>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    uv1ByteStride = accessor.ByteStride(view) ? (accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2);
                }

                if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("JOINTS_0")->second];
                    const tinygltf::BufferView &view = model.bufferViews[accessor.bufferView];
                    bufferJoints = &(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]);
                    jointComponentType = accessor.componentType;
                    jointByteStride = accessor.ByteStride(view) ? (accessor.ByteStride(view) / tinygltf::GetComponentSizeInBytes(jointComponentType)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC4);
                }

                if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("WEIGHTS_0")->second];
                    const tinygltf::BufferView &view = model.bufferViews[accessor.bufferView];
                    bufferWeights = reinterpret_cast<const float *>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    weightByteStride = accessor.ByteStride(view) ? (accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC4);
                }

                bool hasSkin = (bufferJoints && bufferWeights);

                for (size_t v = 0; v < vertexCount; ++v) {
                    Mesh::Vertex vert{};
                    vert.position = vec3(&bufferPos[v * posByteStride]);
                    vert.normal = (vec3(bufferNormals ? vec3(&bufferNormals[v * normByteStride]) : vec3(0.0f))).normal();
                    vert.uv0 = bufferTexCoordSet0 ? vec2(&bufferTexCoordSet0[v * uv0ByteStride]) : vec2(0.0f);
                    vert.uv1 = bufferTexCoordSet1 ? vec2(&bufferTexCoordSet1[v * uv1ByteStride]) : vec2(0.0f);

                    if (hasSkin) {
                        switch (jointComponentType) {
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                                const auto *buf = static_cast<const uint16_t*>(bufferJoints);
                                vert.joint0 = vec4((const float*)&buf[v * jointByteStride]);
                                break;
                            }
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                                const auto *buf = static_cast<const uint8_t*>(bufferJoints);
                                vert.joint0 = vec4((const float*)&buf[v * jointByteStride]);
                                break;
                            }
                            default: {
                                spdlog::error("Joint component type {} not supported!", jointComponentType);
                                break;
                            }
                        }
                    } else {
                        vert.joint0 = vec4(0.0f);
                    }

                    vert.weight0 = hasSkin ? vec4(&bufferWeights[v * weightByteStride]) : vec4(0.0f);

                    // Fix for all zero weights
                    if (vert.weight0.length() == 0.0f)
                        vert.weight0 = vec4(1.0f, 0.0f, 0.0f, 0.0f);

                    vertices.push_back(vert);
                }
            }

            // Indices
            {
                const tinygltf::Accessor &accessor = model.accessors[primitive.indices > -1 ? primitive.indices : 0];
                const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

                indexCount = static_cast<uint32_t>(accessor.count);
                const void *dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                        const auto *buf = static_cast<const uint32_t*>(dataPtr);

                        for (size_t index = 0; index < accessor.count; ++index) indices.push_back(buf[index]);

                        break;
                    }
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
                        const auto *buf = static_cast<const uint16_t*>(dataPtr);

                        for (size_t index = 0; index < accessor.count; index++) indices.push_back(buf[index]);

                        break;
                    }
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
                        const auto *buf = static_cast<const uint8_t*>(dataPtr);

                        for (size_t index = 0; index < accessor.count; index++) indices.push_back(buf[index]);

                        break;
                    }
                    default: {
                        spdlog::error("Index component type {} not supported!", accessor.componentType);
                    }
                }
            }
        }

        return Data{vertices, indices};
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

    std::shared_ptr<Texture> Mesh::getTexture() const {
        return texture;
    }

} // namespace lv