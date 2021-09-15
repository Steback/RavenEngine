#ifndef RAVENENGINE_SCENE_HPP
#define RAVENENGINE_SCENE_HPP


#include <unordered_map>
#include <memory>
#include <string>

#include "entt/entt.hpp"
#include "external/Json.hpp"
#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace re {

    using id_t = entt::entity;

    class Entity;
    class AssetsManager;
    class Skybox;

    /**
     * @brief Scene class
     */
    class Scene : NonCopyable {
        friend Entity;
        friend class RenderSystem;

    public:
        /**
         *
         * @param fileName Scene file name
         * @param assetsManager Valid pointer to AssetsManager
         */
        Scene(std::string fileName);

        ~Scene() override;

        /**
         * @brief Add a new Entity to the scene
         * @param name Entity name
         * @return Pointer to Entity
         */
        std::shared_ptr<Entity> addEntity(const std::string& name);

        /**
         * Add entry from JSON
         * @param entity json object
         * @return
         */
        std::shared_ptr<Entity> addEntity(json& entity);

        /**
         * @brief Load scene from a JSON file
         */
        void load();

        /**
         * @brief Save scene to a JSON file
         * @param fileName
         */
        void save();

        /**
         * @brief Update some entities components.
         *
         * Just Camera component so far
         */
        void update();

        entt::registry& getRegistry();

        std::shared_ptr<Entity> getEntity(id_t id);

        std::shared_ptr<Entity> getEntity(const std::string& name);

        bool loaded() const;

        void setLoaded(bool value);

        void loadSkybox(const std::string& name, VkRenderPass renderPass);

    private:
        std::string fileName;
        entt::registry registry;
        std::unordered_map<id_t, std::shared_ptr<Entity>> entities;
        std::unique_ptr<Skybox> skybox;
        bool wasLoaded{};
    };

} // namespace re



#endif //RAVENENGINE_SCENE_HPP
