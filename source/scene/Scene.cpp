#include "Scene.hpp"

#include "external/Json.hpp"

#include "Skybox.hpp"
#include "entity/Entity.hpp"
#include "files/File.hpp"
#include "files/FilesManager.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/components/MeshRender.hpp"
#include "entity/components/Camera.hpp"


namespace re {

    // TODO: Refactored Scene class and add Doxygen comments
    Scene::Scene(std::string fileName) : fileName(std::move(fileName)) {

    }

    Scene::~Scene() = default;

    std::shared_ptr<Entity> Scene::addEntity(const std::string &name) {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(name, registry.create(), this);
        entities.push_back(entity);
        return entity;
    }

    std::shared_ptr<Entity> Scene::addEntity(json &jsonEntity) {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(jsonEntity, registry.create(), this);
        entities.push_back(entity);
        return entity;
    }

    void Scene::load() {
        File file = files::getFile(fileName);

        json scene;
        file.read(scene);

        for (auto& entityJson : scene["entities"])
            auto entity = addEntity(entityJson);
    }

    void Scene::save() {
        json scene;

        auto& sceneEntities = scene["entities"] = {};
        for (auto& entity : entities)
            sceneEntities.push_back(entity->serialize());

        File file(files::getPath("data") / fileName);
        file.write(scene);
    }

    entt::registry &Scene::getRegistry() {
        return registry;
    }

    std::shared_ptr<Entity> Scene::getEntity(id_t id) {
        return entities[static_cast<uint32_t>(id)];
    }

    std::shared_ptr<Entity> Scene::getEntity(const std::string &name) {
        for (auto& entity : entities) {
            if (name == entity->getName()) return entity;
        }

        return nullptr;
    }

    void Scene::update() {
        for (auto& entity : registry.view<Camera>())
            registry.get<Camera>(entity).update();
    }

    bool Scene::loaded() const {
        return wasLoaded;
    }

    void Scene::setLoaded(bool value) {
        wasLoaded = value;
    }

    void Scene::loadSkybox(const std::string& name, VkRenderPass renderPass) {
        skybox = AssetsManager::getInstance()->loadSkybox(name, renderPass);
    }

} // namespace re
