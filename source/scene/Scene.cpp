#include "Scene.hpp"

#include "external/Json.hpp"

#include "entity/Entity.hpp"
#include "files/File.hpp"
#include "files/FilesManager.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/components/MeshRender.hpp"
#include "entity/components/Camera.hpp"


namespace re {

    Scene::Scene(std::string fileName, std::shared_ptr<AssetsManager> assetsManager)
            : fileName(std::move(fileName)), assetsManager(std::move(assetsManager)) {

    }

    Scene::~Scene() = default;

    std::shared_ptr<Entity> Scene::addEntity(const std::string &name) {
        id_t id = registry.create();
        return entities[id] = std::make_shared<Entity>(name, id, this);
    }

    std::shared_ptr<Entity> Scene::addEntity(json &entity) {
        id_t id = registry.create();
        return entities[id] = std::make_shared<Entity>(entity, id, this);
    }

    void Scene::load() {
        File file = FilesManager::getFile(fileName.c_str());

        json scene;
        file.read(scene);

        for (auto& entityJson : scene["entities"]) {
            auto entity = addEntity(entityJson);

            if (!entityJson["meshRender"].empty()) {
                entity->addComponent<MeshRender>(assetsManager->loadModel(entityJson["meshRender"]["name"].get<std::string>()));
            }
        }

        wasLoaded = true;
    }

    void Scene::save() {
        json scene;

        scene["entities"] = {};
        for (auto& [id, entity] : entities) {
            scene["entities"].push_back(entity->serialize());
        }

        File file(FilesManager::getPath("data/scenes") / fileName);
        file.write(scene);
    }

    entt::registry &Scene::getRegistry() {
        return registry;
    }

    std::shared_ptr<Entity> Scene::getEntity(id_t id) {
        return entities[id];
    }

    std::shared_ptr<Entity> Scene::getEntity(const std::string &name) {
        for (auto& [id, entity] : entities) {
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

} // namespace re
