#include "Scene.hpp"

#include "entity/Entity.hpp"


namespace re {

    Scene::Scene() = default;

    Scene::~Scene() = default;

    std::shared_ptr<Entity> Scene::addEntity(const std::string &name) {
        id_t id = registry.create();
        return entities[id] = std::make_shared<Entity>(name, id, this);
    }

    entt::registry &Scene::getRegistry() {
        return registry;
    }

    std::shared_ptr<Entity> Scene::getEntity(id_t id) {
        return entities[id];
    }

} // namespace re
