#ifndef RAVENENGINE_SCENE_HPP
#define RAVENENGINE_SCENE_HPP


#include <unordered_map>
#include <memory>
#include <string>

#include "entt/entt.hpp"

#include "utils/NonCopyable.hpp"


namespace re {

    using id_t = entt::entity;

    class Entity;

    class Scene : NonCopyable {
        friend Entity;

    public:
        Scene();

        ~Scene() override;

        /**
         * @brief Add a new Entity to the scene
         * @param name Entity name
         * @return Pointer to Entity
         */
        std::shared_ptr<Entity> addEntity(const std::string& name);

        entt::registry& getRegistry();

        std::shared_ptr<Entity> getEntity(id_t id);

    private:
        entt::registry registry;
        std::unordered_map<id_t, std::shared_ptr<Entity>> entities;
    };

} // namespace re



#endif //RAVENENGINE_SCENE_HPP
