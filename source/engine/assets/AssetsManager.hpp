#ifndef RAVENENGINE_ASSETSMANAGER_HPP
#define RAVENENGINE_ASSETSMANAGER_HPP


#include <memory>
#include <unordered_map>
#include <string>

#include "tiny_gltf.h"
#include "stb_image.h"

#include "Mesh.hpp"
#include "engine/logs/Logs.hpp"


namespace re {

    class Device;
    class Model;
    class Texture;
    class Material;
    class Skybox;
    class Asset;

    enum DescriptorSetType {
        UBO = 1,
        TEXTURE = 2,
        MATERIAL = 3
    };

    class AssetsManager : NonCopyable {
        friend class Base;

        explicit AssetsManager(std::shared_ptr<Device> device);

    public:
        ~AssetsManager() override;

        static AssetsManager* getInstance();

        void setupDescriptorsPool(uint32_t imageCount);

        void allocateDescriptorSet(DescriptorSetType type,  VkDescriptorSet* set);

        VkDescriptorSetLayout getDescriptorSetLayout(DescriptorSetType type);

        std::unique_ptr<Skybox> loadSkybox(const std::string &name, VkRenderPass renderPass);

        template<typename T, typename ...Args>
        T* add(std::string name, Args &&... args);

        template<typename T>
        T* get(const std::string& name);

        template<typename T>
        T* get(uint32_t id);

        std::shared_ptr<Device> getDevice();

    private:
        AssetsManager() = default;

        void setupDescriptorSetsLayout();

        void setupMaterialDescriptorsSets();

    private:
        static AssetsManager* singleton;
        VkDescriptorPool descriptorPool{};
        std::shared_ptr<Device> device;
        std::unordered_map<DescriptorSetType, VkDescriptorSetLayout> layouts;
        std::unordered_map<uint32_t, Asset*> assets;
    };

    /**
     * @brief Add a new Assets to AssetsManager. If the Assets already was added, return it.
     * @tparam T Asset type
     * @param name Asset name to hash and save
     * @param args Constructor parameters of T
     * @return Raw pointer to asset
     */
    template<typename T, typename... Args>
    T *AssetsManager::add(std::string name, Args &&... args) {
        uint32_t hashName = std::hash<std::string>()(name);
        if (assets.find(hashName) != assets.end()) return reinterpret_cast<T*>(assets[hashName]);

        T* asset = new T(name, std::forward<Args>(args)...);
        assets[hashName] = asset;

        return asset;
    }

    /**
     *
     * @tparam T Asset type
     * @param name Asset name
     * @return Raw pointer to asset
     */
    template<typename T>
    T *AssetsManager::get(const std::string& name) {
        uint32_t hashName = std::hash<std::string>()(name);
        if (assets.find(hashName) == assets.end()) {
            logs::log(fmt::format("Asset not found {}", name));
            return nullptr;
        }

        return reinterpret_cast<T*>(assets[hashName]);
    }

    /**
     *
     * @tparam T Asset type
     * @param id Asset hashed name
     * @return Raw pointer to asset
     */
    template<typename T>
    T *AssetsManager::get(uint32_t id) {
        if (assets.find(id) == assets.end()) {
            logs::log(fmt::format("Asset not found {}", id));
            return nullptr;
        }

        return reinterpret_cast<T*>(assets[id]);
    }


} // namespace re


#endif //RAVENENGINE_ASSETSMANAGER_HPP
