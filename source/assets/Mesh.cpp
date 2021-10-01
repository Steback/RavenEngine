#include "Mesh.hpp"

#include "spdlog/spdlog.h"

#include "Material.hpp"
#include "AssetsManager.hpp"
#include "render/Device.hpp"
#include "render/buffers/Buffer.hpp"


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
        };
    }

    bool Mesh::Vertex::operator==(const Mesh::Vertex &other) const {
        return position == other.position && normal == other.normal && uv0 == other.uv0;
    }

    /**
     * @brief Construct Mesh from GLTF2 model
     * @param name Asset name
     * @param device Valid pointer to Device
     * @param model TinyGLTF model
     * @param node TinyGLTF node
     */
    Mesh::Mesh(std::string name, std::shared_ptr<Device> device, const tinygltf::Model &model, const tinygltf::Node &node)
            : Asset(std::move(name), Type::MESH), device(std::move(device)) {
#ifdef RE_DEBUG
        logs::log(fmt::format("Load Mesh: {}", this->name));
#endif
        const tinygltf::Mesh& mesh = model.meshes[node.mesh];
        Mesh::Data data = Mesh::loadMesh(model, mesh);

        createVertexBuffer(data.vertices);
        createIndexBuffer(data.indices);
        primitives = std::move(data.primitives);
    }

    Mesh::~Mesh() = default;

    /**
     * @brief Prepare Mesh to be used in Draw
     * @param commandBuffer Valid Command buffer in recording state
     */
    void Mesh::bind(VkCommandBuffer commandBuffer) const {
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    /**
     * @brief Draw Mesh
     * @param commandBuffer Valid Command buffer in recording state
     */
    void Mesh::draw(VkCommandBuffer commandBuffer, VkPipelineLayout layout) const {
        for (auto& primitive : primitives) {
            if (primitive.indexCount > 0) {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 1, 1, &primitive.material->descriptorSet, 0, nullptr);

                Material& material = *primitive.material;
                // Pass material parameters as push constants
                Material::PushConstantBlock pushConstBlockMaterial{};
                pushConstBlockMaterial.colorTextureSet = material.textures[Material::BASE] ? material.texCoordSets.baseColor : -1;
                pushConstBlockMaterial.baseColorFactor = material.baseColorFactor;
                vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushConstBlockMaterial), &pushConstBlockMaterial);

                vkCmdDrawIndexed(commandBuffer, primitive.indexCount, 1, primitive.firstIndex, 0, 0);
            }
        }
    }

    /**
     *
     * @return Current Vertex count of Mesh
     */
    uint32_t Mesh::getVertexCount() const {
        return vertexCount;
    }

    /**
     *
     * @return Current Index count of Mesh. If mesh have no indices the value will be 0.
     */
    uint32_t Mesh::getIndexCount() const {
        return indexCount;
    }

    // TODO: Disable some GLTF vertex attributes(Not used for now)
    /**
     * @brief Load Mesh Data from GLTF2 file
     * @param model TinyGLTF Model
     * @param mesh TinyGLTF Mesh
     * @return Mesh Data(Vertices and Indices)
     */
    Mesh::Data Mesh::loadMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh) {
        std::vector<Mesh::Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Primitive> primitives;

        for (auto gltfPrimitive : mesh.primitives) {
            auto firstIndex = static_cast<uint32_t>(indices.size());
            auto vertexStart = static_cast<uint32_t>(vertices.size());
            uint32_t indexCount = 0;

            // Vertices
            {
                uint32_t vertexCount = 0;
                const float *bufferPos = nullptr;
                const float *bufferNormals = nullptr;
                const float *bufferTexCoordSet0 = nullptr;

                int posByteStride;
                int normByteStride;
                int uv0ByteStride;

                if (gltfPrimitive.attributes.find("POSITION") != gltfPrimitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[gltfPrimitive.attributes.find("POSITION")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferPos = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    vertexCount = static_cast<uint32_t>(accessor.count);
                    posByteStride = accessor.ByteStride(view) ? (int)(accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);
                }

                if (gltfPrimitive.attributes.find("NORMAL") != gltfPrimitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[gltfPrimitive.attributes.find("NORMAL")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferNormals = reinterpret_cast<const float *>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    normByteStride = accessor.ByteStride(view) ? (int)(accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);
                }

                if (gltfPrimitive.attributes.find("TEXCOORD_0") != gltfPrimitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[gltfPrimitive.attributes.find("TEXCOORD_0")->second];
                    const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                    bufferTexCoordSet0 = reinterpret_cast<const float *>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    uv0ByteStride = accessor.ByteStride(view) ? (int)(accessor.ByteStride(view) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2);
                }

                for (size_t v = 0; v < vertexCount; ++v) {
                    Mesh::Vertex vert{};
                    vert.position = vec3(&bufferPos[v * posByteStride]);
                    vert.normal = (vec3(bufferNormals ? vec3(&bufferNormals[v * normByteStride]) : vec3(0.0f))).normal();
                    vert.uv0 = bufferTexCoordSet0 ? vec2(&bufferTexCoordSet0[v * uv0ByteStride]) : vec2(0.0f);

                    vertices.push_back(vert);
                }
            }

            // Indices
            {
                const tinygltf::Accessor &accessor = model.accessors[gltfPrimitive.indices > -1 ? gltfPrimitive.indices : 0];
                const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

                indexCount += static_cast<uint32_t>(accessor.count);

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

            Primitive primitive{};
            primitive.firstIndex = firstIndex;
            primitive.indexCount = indexCount;

            tinygltf::Material material = model.materials[gltfPrimitive.material];
            primitive.material = AssetsManager::getInstance()->add<Material>(material.name, model, material);

            primitives.push_back(primitive);
        }

        return Data{vertices, indices, primitives};
    }

    void Mesh::createVertexBuffer(const std::vector<Vertex> &vertices) {
        vertexCount = vertices.size();
        VkDeviceSize size = sizeof(vertices[0]) * vertexCount;

        Buffer stagingBuffer(device->getAllocator(), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.writeTo((void*)vertices.data());

        vertexBuffer = std::make_unique<Buffer>(device->getAllocator(), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        device->copyBuffer(stagingBuffer, *vertexBuffer, size);
    }

    void Mesh::createIndexBuffer(const std::vector<uint32_t>& indices) {
        indexCount = indices.size();

        VkDeviceSize size = sizeof(indices[0]) * indexCount;

        Buffer stagingBuffer(device->getAllocator(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.writeTo((void*)indices.data());

        indexBuffer = std::make_unique<Buffer>(device->getAllocator(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        device->copyBuffer(stagingBuffer, *indexBuffer, size);
    }

} // namespace lv