#include "Entity.hpp"

#include "assets/AssetsManager.hpp"
#include "components/Transform.hpp"
#include "components/MeshRender.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"


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

        if (hasComponent<Light>()) {
            entity["light"] = getComponent<Light>().serialize();
        }

        return entity;
    }

    /**
     *
     * @param entity JSON Serialized data to set Entity
     */
    void Entity::serialize(json &entity) {
        name = entity["name"];

        if (!entity["transform"].empty())
            addComponent<Transform>(entity["transform"]);

        if (!entity["camera"].empty())
            addComponent<Camera>(entity["camera"]);

        if (!entity["light"].empty())
            addComponent<Light>(entity["light"]);

        // TODO: Use promises to load mesh in other thread.
        if (!entity["meshRender"].empty()) {
            auto modelName = entity["meshRender"]["name"].get<std::string>();
            addComponent<MeshRender>(AssetsManager::getInstance()->add<Model>(modelName, modelName));
        }

    }

} // namespace re

