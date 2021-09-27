#include "Model.hpp"

#include "AssetsManager.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "math/Matrix3.hpp"
#include "utils/Utils.hpp"
#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"


namespace re {

    /**
     *
     * @return Matrix4 of Node local space.
     */
    Matrix4 Model::Node::getLocalMatrix() const {
        Matrix3 rotationMatrix = rotation.unit().rotationMatrix();

        return Matrix4(
                { scale.x * rotationMatrix[0][0], scale.x * rotationMatrix[1][0], scale.x * rotationMatrix[2][0], 0.0f },
                { scale.y * rotationMatrix[0][1], scale.y * rotationMatrix[1][1], scale.y * rotationMatrix[2][1], 0.0f },
                { scale.z * rotationMatrix[0][2], scale.z * rotationMatrix[1][2], scale.z * rotationMatrix[2][2], 0.0f },
                { translation.x, translation.y, translation.z, 1.0f }
            ) * matrix;
    }

    /**
     * @brief Construct Model from GLTF2 file
     * @param name Model name
     * @param fileName Model file name
     */
    Model::Model(std::string name, const std::string& fileName) : Asset(std::move(name)) {
#ifdef RE_DEBUG
        logs::error(fmt::format("Load model: {}", fileName));
#endif
        File file = files::getFile(fileName);
        tinygltf::Model gltfModel;
        tinygltf::TinyGLTF gltfContext;

        bool fileLoaded;
        if (file.getExtension() == ".glb")
            fileLoaded = gltfContext.LoadBinaryFromFile(&gltfModel, nullptr, nullptr, file.getPath());
        else
            fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, nullptr, nullptr, file.getPath());

        if (fileLoaded) {
            const tinygltf::Scene& scene = gltfModel.scenes[0];

            nodes.resize(gltfModel.nodes.size());

            for (int index : scene.nodes) {
                const tinygltf::Node node = gltfModel.nodes[index];
                loadNode(gltfModel, -1, node, index);
            }
        }
    }

    Model::~Model() = default;

    /**
     * @brief Calculate a Node matrix in Local Space with the parent matrix
     * @param index Node index
     */
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

    /**
     * @brief Prepare and use all necessary stuff to render Model
     * @param commandBuffer Valid Command Buffer in recording state
     * @param layout Valid Vulkan pipeline layout to send data to Shader
     * @param ubo Model Ubo(Uniform Buffer Object) to save all Node data
     */
    void Model::render(VkCommandBuffer commandBuffer, VkPipelineLayout layout, Ubo& ubo) {
        for (auto& node : nodes) {
            if (node.mesh) {
                ubo.nodeMatrix = getNodeMatrix(node.index);

                std::vector<VkDescriptorSet> sets = {node.mesh->getMaterial()->descriptorSet};
                vkCmdBindDescriptorSets(commandBuffer,
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        layout, 1, static_cast<uint32_t>(sets.size()), sets.data(),
                                        0, nullptr);


                Material& material = *node.mesh->material;
                // Pass material parameters as push constants
                Material::PushConstantBlock pushConstBlockMaterial{};
                pushConstBlockMaterial.colorTextureSet = material.textures[Material::BASE] ? material.texCoordSets.baseColor : -1;
                pushConstBlockMaterial.baseColorFactor = material.baseColorFactor;

                vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushConstBlockMaterial), &pushConstBlockMaterial);

                node.mesh->bind(commandBuffer);
                node.mesh->draw(commandBuffer);
            }
        }
    }

    /**
     *
     * @param index Node index
     * @return Reference of Node
     */
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
            newNode.mesh = AssetsManager::getInstance()->add<Mesh>(node.name, AssetsManager::getInstance()->getDevice(), model, node);
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

} // namespace re
