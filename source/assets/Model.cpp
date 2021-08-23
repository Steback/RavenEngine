#include "Model.hpp"

#include "AssetsManager.hpp"
#include "math/Matrix3.hpp"


namespace re {

    Matrix4 Model::Node::getLocalMatrix() const {
        Matrix3 rotationMatrix = rotation.getRotationMatrix();

        return Matrix4(
                { scale.x * rotationMatrix[0][0], scale.x * rotationMatrix[1][0], scale.x * rotationMatrix[2][0], 0.0f },
                { scale.y * rotationMatrix[0][1], scale.y * rotationMatrix[1][1], scale.y * rotationMatrix[2][1], 0.0f },
                { scale.z * rotationMatrix[0][2], scale.z * rotationMatrix[1][2], scale.z * rotationMatrix[2][2], 0.0f },
                { translation.x, translation.y, translation.z, 1.0f }
            ) * matrix;
    }

    Model::Model(AssetsManager* assetsManager, const tinygltf::Model &model) : assetsManager(assetsManager) {
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
        uint32_t parentIndex = node.parent;

        while (parentIndex > -1) {
            const Node& currentParent = nodes[parentIndex];
            nodeMatrix = currentParent.getLocalMatrix() * nodeMatrix;
            parentIndex = currentParent.parent;
        }

        return nodeMatrix;
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
            newNode.translation = vec3((const float*)node.translation.data());
        }

        if (node.rotation.size() == 4) {
            newNode.rotation = quat((const float*)node.rotation.data());
        }

        if (node.scale.size() == 3) {
            newNode.scale = vec3((const float*)node.scale.data());
        }

        if (node.matrix.size() == 16) {
            newNode.matrix = mat4((const float*)node.matrix.data());
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

} // namespace re
