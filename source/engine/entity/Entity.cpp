#include "Entity.hpp"

#include "nameof.hpp"

#include "engine/assets/AssetsManager.hpp"
#include "engine/entity/components/Transform.hpp"
#include "engine/entity/components/MeshRender.hpp"
#include "engine/entity/components/Camera.hpp"
#include "engine/entity/components/Light.hpp"


namespace re {

    /**
     *
     * @param name Entity name
     * @param id Valid EnTT id
     * @param scene Valid pointer to Scene
     */
    Entity::Entity(std::string name, id_t id, Scene *scene) : name(std::move(name)), id(id), scene(scene) {

    }

    /**
     *
     * @param entity Entity serialized data in JSON
     * @param id Valid EnTT id
     * @param scene Valid pointer to Scene
     */
    Entity::Entity(json &entity, id_t id, Scene *scene) : scene(scene), id(id) {
        serialize(entity);
    }

    Entity::~Entity() = default;

    /**
     *
     * @return EnTT id
     */
    id_t Entity::getId() const {
        return id;
    }

    /**
     *
     * @return Entity name
     */
    const std::string &Entity::getName() const {
        return name;
    }

    /**
     *
     * @param name_ New name to set to Entity
     */
    void Entity::setName(const std::string &name_) {
        Entity::name = name_;
    }

    /**
     *
     * @return JSON object with Entity serialized data
     */
    json Entity::serialize() {
        json entity = {};
        entity[std::string(NAMEOF(name))] = name;

        // TODO: Find a way to it this dynamically
        if (hasComponent<Transform>())
            entity[std::string(NAMEOF_SHORT_TYPE(Transform))] = getComponent<Transform>().serialize();

        if (hasComponent<MeshRender>())
            entity[std::string(NAMEOF_SHORT_TYPE(MeshRender))] = getComponent<MeshRender>().serialize();

        if (hasComponent<Camera>())
            entity[std::string(NAMEOF_SHORT_TYPE(Camera))] = getComponent<Camera>().serialize();

        if (hasComponent<Light>())
            entity[std::string(NAMEOF_SHORT_TYPE(Light))] = getComponent<Light>().serialize();

        return entity;
    }

    /**
     *
     * @param entity JSON Serialized data to set Entity
     */
    void Entity::serialize(json &entity) {
        name = entity[std::string(NAMEOF(name))];

        std::string nameComponent = std::string(NAMEOF_SHORT_TYPE(Transform));
        if (!entity[nameComponent].empty())
            addComponent<Transform>(entity[nameComponent]);

        nameComponent = std::string(NAMEOF_SHORT_TYPE(MeshRender));
        if (!entity[nameComponent].empty())
            addComponent<MeshRender>(entity[nameComponent]);

        nameComponent = std::string(NAMEOF_SHORT_TYPE(Camera));
        if (!entity[nameComponent].empty())
            addComponent<Camera>(entity[nameComponent]);

        nameComponent = std::string(NAMEOF_SHORT_TYPE(Light));
        if (!entity[nameComponent].empty())
            addComponent<Light>(entity[nameComponent]);
    }

    std::shared_ptr<Entity> Entity::addChild(const std::string &childName) {
        std::shared_ptr<Entity> child = std::make_shared<Entity>(childName, scene->registry.create(), scene);
        children.push_back(child);
        return child;
    }

    std::shared_ptr<Entity> Entity::getChild(const std::string &childName) {
        for (auto& child : children) {
            if (child->name == childName) return child;

            if (!child->children.empty()) {
                return child->getChild(childName);
            }
        }

        return nullptr;
    }

    void Entity::setParent(std::shared_ptr<Entity> newParent) {
        parent = std::move(newParent);
    }

    std::shared_ptr<Entity> Entity::getParent() {
        return parent;
    }

    std::vector<std::shared_ptr<Entity>> &Entity::getChildren() {
        return children;
    }

} // namespace re
