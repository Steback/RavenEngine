#include "Entity.hpp"


namespace re {

    Entity::Entity(std::string name, id_t id, Scene *scene) : name(std::move(name)), id(id), scene(scene) {

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

} // namespace re

