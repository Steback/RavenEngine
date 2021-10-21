#include "Scene.hpp"

#include "engine/external/Json.hpp"
#include "nameof.hpp"

#include "Skybox.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/files/File.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/assets/AssetsManager.hpp"
#include "engine/entity/components/MeshRender.hpp"
#include "engine/entity/components/Camera.hpp"


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

        for (auto& entityJson : scene[std::string(NAMEOF(entities))])
            auto entity = addEntity(entityJson);

        // TODO: Temporally solution. Need to find a better approach
        while (true) {
            auto view = registry.view<MeshRender>();
            bool modelsLoaded = std::all_of(view.begin(), view.end(), [=, this](id_t id){
                auto& meshRender = view.get<MeshRender>(id);
                return meshRender.enable;
            });
            if (modelsLoaded) break;
        }
    }

    void Scene::save() {
        json scene;

        auto& sceneEntities = scene[std::string(NAMEOF(entities))] = {};
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
