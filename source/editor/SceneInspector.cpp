#include "SceneInspector.hpp"

#include "imgui.h"

#include "engine/scene/Scene.hpp"
#include "engine/entity/Entity.hpp"
#include "engine/entity/components/Transform.hpp"


namespace re {

    SceneInspector::SceneInspector(std::shared_ptr<Scene> scene) : scene(std::move(scene)) {

    }

    void SceneInspector::drawScene() {
        // Window itself popup
        if (ImGui::BeginPopupContextWindow()) {
            popupContext();
            ImGui::EndPopup();
        }

        for (auto& entity : scene->getEntities()) {
            showEntityTree(entity);
        }
    }

    void SceneInspector::showEntityTree(const std::shared_ptr<Entity>& entity) {
        bool nodeOpen = ImGui::TreeNodeEx(entity->getName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);

        if (ImGui::BeginPopupContextItem()) {
            popupContext(entity);
            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked()) selectedEntity = entity;

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
        if (ImGui::Selectable("Add Entity")) { addEntity(entity); };
    }

    void SceneInspector::addEntity(const std::shared_ptr<Entity>& entity) {
        if (entity) {
            entity->addChild("Entity")->addComponent<Transform>(vec3{}, vec3{}, vec3{});
        } else {
            scene->addEntity("Entity")->addComponent<Transform>(vec3{}, vec3{}, vec3{});
        }
    }

} // namespace re
