#include "Entity.hpp"

#include "components/Transform.hpp"
#include "components/MeshRender.hpp"
#include "components/Camera.hpp"


namespace re {

    Entity::Entity(std::string name, id_t id, Scene *scene) : name(std::move(name)), id(id), scene(scene) {

    }

    Entity::Entity(json &entity, id_t id, Scene *scene) : scene(scene), id(id) {
        serialize(entity);
    }

    Entity::~Entity() = default;

    id_t Entity::getId() const {
        return id;
    }

    const std::string &Entity::getName() const {
        return name;
    }

    void Entity::setName(const std::string &name_) {
        Entity::name = name_;
    }

    json Entity::serialize() {
        json entity = {};
        entity["name"] = name;

        // TODO: Find a way to it this dynamically
        if (hasComponent<Transform>())
            entity["transform"] = getComponent<Transform>().serialize();

        if (hasComponent<MeshRender>()) {
            entity["meshRender"] = getComponent<MeshRender>().serialize();
        }

        if (hasComponent<Camera>()) {
            entity["camera"] = getComponent<Camera>().serialize();
        }

        return entity;
    }

    void Entity::serialize(json &entity) {
        name = entity["name"];

        if (!entity["transform"].empty())
            addComponent<Transform>(entity["transform"]);

        if (!entity["camera"].empty())
            addComponent<Camera>(entity["camera"]);
    }

} // namespace re

