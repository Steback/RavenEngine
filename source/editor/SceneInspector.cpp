#include "SceneInspector.hpp"

#include "engine/scene/Scene.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/components/Transform.hpp"
#include "engine/render/ui/ImElements.hpp"


namespace re {

    SceneInspector::SceneInspector(std::shared_ptr<Scene> scene) : scene(std::move(scene)) {

    }

    void SceneInspector::drawScene() {
        // Window itself popup
        ui::imPopupContextWindow([&, this]{
            popupContext();
        });

        for (auto& entity : scene->getEntities()) {
            showEntityTree(entity);
        }
    }

    void SceneInspector::showEntityTree(const std::shared_ptr<Entity>& entity) {
        bool nodeOpen = ui::imTreeNodeEx(entity->getName(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);

        ui::imPopupContextItem([&, this]{
            popupContext(entity);
        });

        if (ui::imIsClicked()) selectedEntity = entity;

        if (nodeOpen) {
            for (auto& child : entity->getChildren()) {
                showEntityTree(child);
            }
            ImGui::TreePop();
        }
    }

    std::shared_ptr<Entity> SceneInspector::getSelectedEntity() {
        return selectedEntity;
    }

    void SceneInspector::popupContext(const std::shared_ptr<Entity> &entity) {
        ui::imSelectable("Add Entity", [&, this]{ addEntity(entity); });
    }

    void SceneInspector::addEntity(const std::shared_ptr<Entity>& entity) {
        if (entity) {
            entity->addChild("Entity")->addComponent<Transform>(vec3{}, vec3{}, vec3{});
        } else {
            scene->addEntity("Entity")->addComponent<Transform>(vec3{}, vec3{}, vec3{});
        }
    }

} // namespace re
