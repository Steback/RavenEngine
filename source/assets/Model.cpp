#include "Model.hpp"

#include <unordered_map>

#include "AssetsManager.hpp"
#include "Texture.hpp"
#include "math/Matrix3.hpp"
#include "utils/Utils.hpp"
#include "utils/Macros.hpp"
#include "render/RenderSystem.hpp"


namespace std {

    template <>
    struct hash<re::Mesh::Vertex> {
        size_t operator()(re::Mesh::Vertex const &vertex) const {
            size_t seed = 0;
            hash<float> hasher;
            re::hashCombine(seed, hasher(vertex.position.x), hasher(vertex.position.y), hasher(vertex.position.z));
            re::hashCombine(seed, hasher(vertex.normal.x), hasher(vertex.normal.y), hasher(vertex.normal.z));
            re::hashCombine(seed, hasher(vertex.uv.x), hasher(vertex.uv.y));
            return seed;
        }
    };

}  // namespace std

namespace re {

    Matrix4 Model::Node::getLocalMatrix() const {
        Matrix3 rotationMatrix = rotation.unit().getRotationMatrix();

        return Matrix4(
                { scale.x * rotationMatrix[0][0], scale.x * rotationMatrix[1][0], scale.x * rotationMatrix[2][0], 0.0f },
                { scale.y * rotationMatrix[0][1], scale.y * rotationMatrix[1][1], scale.y * rotationMatrix[2][1], 0.0f },
                { scale.z * rotationMatrix[0][2], scale.z * rotationMatrix[1][2], scale.z * rotationMatrix[2][2], 0.0f },
                { translation.x, translation.y, translation.z, 1.0f }
            ) * matrix;
    }

    Model::Model(AssetsManager* assetsManager, std::string name, const tinygltf::Model &model)
            : assetsManager(assetsManager), name(std::move(name)) {
        const tinygltf::Scene& scene = model.scenes[0];

        nodes.resize(model.nodes.size());

        for (int index : scene.nodes) {
            const tinygltf::Node node = model.nodes[index];
            loadNode(model, -1, node, index);
        }
    }

    Model::Model(AssetsManager* assetsManager, std::string name, const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes)
            : assetsManager(assetsManager), name(std::move(name)) {
        Mesh::Data data;
        std::unordered_map<Mesh::Vertex, uint32_t> uniqueVertices{};
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Mesh::Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2],
                    };
                }

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = CAST_U32(data.vertices.size());
                    data.vertices.push_back(vertex);
                }

                data.indices.push_back(uniqueVertices[vertex]);
            }
        }

        Node node{};
        node.index = 0;
        node.matrix = mat4(1.0f);
        node.mesh = assetsManager->addMesh(shapes[0].name, data);

        nodes.push_back(node);
    }

    Model::~Model() = default;

    Matrix4 Model::getNodeMatrix(size_t index) const {
        const Node& node = nodes[index];
        Matrix4 nodeMatrix = node.getLocalMatrix();
        int32_t parentIndex = node.parent;

        while (parentIndex > -1) {
            const Node& currentParent = nodes[parentIndex];
            nodeMatrix = currentParent.getLocalMatrix() * nodeMatrix;
            parentIndex = currentParent.parent;
        }

        return nodeMatrix;
    }

    // TODO: Find another way to calculate MVP * Node Matrix and send it to Shaders
    void Model::render(VkCommandBuffer commandBuffer, VkPipelineLayout layout, PushConstant& push) {
        for (auto& node : nodes) {
            if (node.mesh) {
                push.node = getNodeMatrix(node.index);

                vkCmdPushConstants(
                        commandBuffer,
                        layout,
                        VK_SHADER_STAGE_VERTEX_BIT,
                        0,
                        sizeof(PushConstant),
                        &push
                );

                node.mesh->bind(commandBuffer);

                std::vector<VkDescriptorSet> sets = {node.mesh->getTexture()->getDescriptorSet()};
                vkCmdBindDescriptorSets(commandBuffer,
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        layout, 0, static_cast<uint32_t>(sets.size()), sets.data(),
                                        0, nullptr);

                node.mesh->draw(commandBuffer);
            }
        }
    }

    Model::Node &Model::getNode(uint32_t index) {
        return nodes[index];
    }

    void Model::loadNode(const tinygltf::Model &model, int32_t parentIndex, const tinygltf::Node &node, uint32_t nodeIndex) {
        Node newNode{};
        newNode.index = nodeIndex;
        newNode.parent = parentIndex;
        newNode.name = node.name;
        newNode.matrix = Matrix4(1.0f);

        if (node.translation.size() == 3) {
            newNode.translation = vec3(node.translation.data());
        }

        if (node.rotation.size() == 4) {
            newNode.rotation = quat(node.rotation.data());
        }

        if (node.scale.size() == 3) {
            newNode.scale = vec3(node.scale.data());
        }

        if (node.matrix.size() == 16) {
            newNode.matrix = mat4(node.matrix.data());
        }

        if (node.mesh > -1) {
            newNode.mesh = assetsManager->addMesh(model, node);
        }

        if (parentIndex > -1) {
            Node& parent = nodes[parentIndex];
            parent.children.push_back(newNode.index);
            newNode.parent = static_cast<int32_t>(parent.index);
        }

        nodes[nodeIndex] = newNode;

        for (auto& childrenIndex : node.children) {
            loadNode(model, static_cast<int32_t>(newNode.index), model.nodes[childrenIndex], childrenIndex);
        }
    }

    std::string Model::getName() const {
        return name;
    }

} // namespace re
