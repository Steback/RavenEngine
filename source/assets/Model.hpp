#ifndef RAVENENGINE_MODEL_HPP
#define RAVENENGINE_MODEL_HPP


#include <memory>
#include <vector>
#include <string>

#include "tiny_gltf.h"
#include "vulkan/vulkan.h"

#include "Asset.hpp"
#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"


namespace re {

    class Device;
    class Mesh;
    class AssetsManager;

    class Model : public Asset {
    public:
        struct Node {
            int32_t parent{-1};
            uint32_t index;
            std::string name;
            std::vector<uint32_t> children;
            Mesh* mesh;
            Vector3 translation;
            Vector3 scale{1.0f};
            Quaternion rotation;

            [[nodiscard]] Matrix4 getLocalMatrix() const;
        };

        struct Ubo {
            mat4 nodeMatrix{1.0f};
        };

    public:
        Model(std::string name, const std::string& fileName);

        ~Model() override;

        [[nodiscard]] Matrix4 getNodeMatrix(size_t index) const;

        void render(VkCommandBuffer commandBuffer, VkPipelineLayout layout, Ubo& ubo);

        Node& getNode(uint32_t index);

    private:
        void loadNode(const tinygltf::Model& model, int32_t parentIndex, const tinygltf::Node& node, uint32_t nodeIndex);

    private:
        std::vector<Node> nodes;
    };

} // namespace re


#endif //RAVENENGINE_MODEL_HPP
