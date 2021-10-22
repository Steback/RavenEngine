#ifndef RAVENENGINE_SCENE_HPP
#define RAVENENGINE_SCENE_HPP


#include <vector>
#include <memory>
#include <string>

#include "entt/entt.hpp"
#include "engine/external/Json.hpp"
#include "vulkan/vulkan.h"

#include "engine/utils/NonCopyable.hpp"


namespace re {

    using id_t = entt::entity;

    class Entity;
    class AssetsManager;
    class Skybox;

    class Scene : NonCopyable {
        friend Entity;
        friend class RenderSystem;

    public:
        Scene();

        explicit Scene(std::string fileName);

        ~Scene() override;

        std::shared_ptr<Entity> addEntity(const std::string& name);

        std::shared_ptr<Entity> addEntity(json& entity);

        void load();

        void save();

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
        std::vector<std::shared_ptr<Entity>> entities;
        std::unique_ptr<Skybox> skybox;
        bool wasLoaded{};
    };

} // namespace re



#endif //RAVENENGINE_SCENE_HPP
