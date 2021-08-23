#ifndef RAVENENGINE_ASSETSMANAGER_HPP
#define RAVENENGINE_ASSETSMANAGER_HPP


#include <memory>
#include <unordered_map>
#include <string>

#include "tiny_gltf.h"

#include "Mesh.hpp"


namespace re {

    class Device;
    class Model;

    /**
     * Assets Manager class
     */
    class AssetsManager {
    public:
        /**
         * Default Constructor
         * @param device shared_ptr of Device class
         */
        explicit AssetsManager(std::shared_ptr<Device> device);

        ~AssetsManager();

        /**
         *
         * @param fileName Name of GLTF2 file
         * @return shared_ptr of Model
         */
        std::shared_ptr<Model> loadModel(const std::string& fileName);

        /**
         * @brief Load Mesh from raw data
         * @param name Name of mesh
         * @param data Mesh vertices and Indices
         * @return share_ptr of Mesh
         */
        std::shared_ptr<Mesh> addMesh(const std::string& name, const Mesh::Data& data);

        /**
         * @brief Load Mesh from GLTF Mesh
         * @param model GLTF Model
         * @param node  GLTF node
         * @return shared_ptr of created Mesh
         */
        std::shared_ptr<Mesh> addMesh(const tinygltf::Model& model, const tinygltf::Node& node);

        std::shared_ptr<Model> getModel(uint32_t name);

        std::shared_ptr<Mesh> getMesh(uint32_t name);

    private:
        std::shared_ptr<Device> device;
        std::unordered_map<uint32_t, std::shared_ptr<Model>> models;
        std::unordered_map<uint32_t, std::shared_ptr<Mesh>> meshes;
    };

} // namespace re


#endif //RAVENENGINE_ASSETSMANAGER_HPP
