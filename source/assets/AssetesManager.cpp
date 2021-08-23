#include "AssetsManager.hpp"

#include "spdlog/spdlog.h"

#include "render/Device.hpp"
#include "files/FilesManager.hpp"
#include "files/File.hpp"
#include "utils/Macros.hpp"
#include "Model.hpp"


namespace re {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {

    }

    AssetsManager::~AssetsManager() = default;

    std::shared_ptr<Model> AssetsManager::loadModel(const std::string &fileName) {
        File file = FilesManager::getFile(fileName.c_str());
        tinygltf::Model gltfModel;
        tinygltf::TinyGLTF gltfContext;
        std::string error;
        std::string warning;

        bool fileLoaded;
        if (file.getExtension() == ".glb")
            fileLoaded = gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, file.getPath());
        else
            fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, file.getPath());

        if (fileLoaded)
            return models[std::hash<std::string>()(file.getName())] = std::make_shared<Model>(this, gltfModel);

        return nullptr;
    }

    std::shared_ptr<Mesh> AssetsManager::addMesh(const std::string &name, const Mesh::Data& data) {
        return meshes[std::hash<std::string>()(name)] = std::make_shared<Mesh>(device, data);
    }

    std::shared_ptr<Mesh> AssetsManager::addMesh(const tinygltf::Model &model, const tinygltf::Node &node) {
        const tinygltf::Mesh& mesh = model.meshes[node.mesh];

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

        return meshes[std::hash<std::string>()(node.name)] = std::make_shared<Mesh>(device, Mesh::Data{vertices, indices});
    }

} // namespace re