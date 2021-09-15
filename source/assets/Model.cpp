#include "Model.hpp"

#include <unordered_map>

#include "AssetsManager.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "math/Matrix3.hpp"
#include "utils/Utils.hpp"
#include "render/RenderSystem.hpp"


namespace std {

    template <>
    struct hash<re::Mesh::Vertex> {
        size_t operator()(re::Mesh::Vertex const &vertex) const {
            size_t seed = 0;
            hash<float> hasher;
            re::hashCombine(seed, hasher(vertex.position.x), hasher(vertex.position.y), hasher(vertex.position.z));
            re::hashCombine(seed, hasher(vertex.normal.x), hasher(vertex.normal.y), hasher(vertex.normal.z));
            re::hashCombine(seed, hasher(vertex.uv0.x), hasher(vertex.uv0.y));
            re::hashCombine(seed, hasher(vertex.uv1.x), hasher(vertex.uv1.y));
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

    Model::Model(std::string name, const tinygltf::Model &model) : name(std::move(name)) {
        const tinygltf::Scene& scene = model.scenes[0];

        nodes.resize(model.nodes.size());

        for (int index : scene.nodes) {
            const tinygltf::Node node = model.nodes[index];
            loadNode(model, -1, node, index);
        }
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
    void Model::render(VkCommandBuffer commandBuffer, VkPipelineLayout layout, UboModel& uboModel) {
        for (auto& node : nodes) {
            if (node.mesh) {
                uboModel.node = getNodeMatrix(node.index);

                std::vector<VkDescriptorSet> sets = {node.mesh->getMaterial()->descriptorSet};
                vkCmdBindDescriptorSets(commandBuffer,
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        layout, 1, static_cast<uint32_t>(sets.size()), sets.data(),
                                        0, nullptr);


                Material& material = *node.mesh->material;
                // Pass material parameters as push constants
                PushConstBlockMaterial pushConstBlockMaterial{};
                pushConstBlockMaterial.emissiveFactor = material.emissiveFactor;
                // To save push constant space, availabilty and texture coordiante set are combined
                // -1 = texture not used for this material, >= 0 texture used and index of texture coordinate set
                pushConstBlockMaterial.colorTextureSet = material.baseColorTexture ? material.texCoordSets.baseColor : -1;
                pushConstBlockMaterial.normalTextureSet = material.normalTexture ? material.texCoordSets.normal : -1;
                pushConstBlockMaterial.occlusionTextureSet = material.occlusionTexture ? material.texCoordSets.occlusion : -1;
                pushConstBlockMaterial.emissiveTextureSet = material.emissiveTexture ? material.texCoordSets.emissive : -1;
                pushConstBlockMaterial.alphaMask = static_cast<float>(material.alphaMode == Material::AlphaMode::MASK);
                pushConstBlockMaterial.alphaMaskCutoff = material.alphaCutoff;

                // TODO: glTF specs states that metallic roughness should be preferred, even if specular glosiness is present
                if (material.pbrWorkflows.metallicRoughness) {
                    // Metallic roughness workflow
                    pushConstBlockMaterial.workflow = static_cast<float>(Material::PBRWorkflows::METALLIC_ROUGHNESS);
                    pushConstBlockMaterial.baseColorFactor = material.baseColorFactor;
                    pushConstBlockMaterial.metallicFactor = material.metallicFactor;
                    pushConstBlockMaterial.roughnessFactor = material.roughnessFactor;
                    pushConstBlockMaterial.PhysicalDescriptorTextureSet = material.metallicRoughnessTexture != nullptr ? material.texCoordSets.metallicRoughness : -1;
                    pushConstBlockMaterial.colorTextureSet = material.baseColorTexture != nullptr ? material.texCoordSets.baseColor : -1;
                }

                if (material.pbrWorkflows.specularGlossiness) {
                    // Specular glossiness workflow
                    pushConstBlockMaterial.workflow = static_cast<float>(Material::PBRWorkflows::SPECULAR_GLOSINESS);
                    pushConstBlockMaterial.PhysicalDescriptorTextureSet = material.extension.specularGlossinessTexture != nullptr ? material.texCoordSets.specularGlossiness : -1;
                    pushConstBlockMaterial.colorTextureSet = material.extension.diffuseTexture != nullptr ? material.texCoordSets.baseColor : -1;
                    pushConstBlockMaterial.diffuseFactor = material.extension.diffuseFactor;
                    pushConstBlockMaterial.specularFactor = vec4(material.extension.specularFactor.x, material.extension.specularFactor.y, material.extension.specularFactor.z, 1.0f);
                }

                vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstBlockMaterial), &pushConstBlockMaterial);

                node.mesh->bind(commandBuffer);
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
            newNode.mesh = AssetsManager::getInstance()->addMesh(model, node);
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
