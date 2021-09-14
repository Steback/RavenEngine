#ifndef RAVENENGINE_MODEL_HPP
#define RAVENENGINE_MODEL_HPP


#include <memory>
#include <vector>
#include <string>

#include "tiny_gltf.h"
#include "vulkan/vulkan.h"

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"


namespace re {

    struct UboModel;

    class Device;
    class Mesh;
    class AssetsManager;

    /**
     * @brief Model interface class
     */
    class Model {
    public:
        /**
         * @brief Model node struct
         */
        struct Node {
            int32_t parent{-1};
            uint32_t index;
            std::string name;
            std::vector<uint32_t> children;
            std::shared_ptr<Mesh> mesh;
            Matrix4 matrix;
            Vector3 translation;
            Vector3 scale{1.0f};
            Quaternion rotation;

            /**
             *
             * @return Node matrix in local space
             */
            [[nodiscard]] Matrix4 getLocalMatrix() const;

        };

    public:
        /**
         * @brief Default constructor. Create Model object from GLTF2 file
         * @param assetsManager Pointer to Assets Manager
         * @param model TinyGLTF model wasLoaded
         */
        Model(AssetsManager* assetsManager, std::string name, const tinygltf::Model &model);

        ~Model();

        /**
         *
         * @param index Node index
         * @return Node matrix in world space. Multiply with the patent matrix
         */
        [[nodiscard]] Matrix4 getNodeMatrix(size_t index) const;

        /**
         *
         * @param commandBuffer Command uboBuffer in recording state
         * @param layout Valid pipeline layout
         */
        void render(VkCommandBuffer commandBuffer, VkPipelineLayout layout, UboModel& uboModel);

        Node& getNode(uint32_t index);

        [[nodiscard]] std::string getName() const;

    private:
        void loadNode(const tinygltf::Model& model, int32_t parentIndex, const tinygltf::Node& node, uint32_t nodeIndex);

    private:
        std::string name;
        std::vector<Node> nodes;
        AssetsManager* assetsManager{};
    };

} // namespace re


#endif //RAVENENGINE_MODEL_HPP
