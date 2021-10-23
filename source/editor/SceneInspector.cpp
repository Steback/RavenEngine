#include "SceneInspector.hpp"

#include "imgui.h"

#include "engine/scene/Scene.hpp"
#include "engine/entity/Entity.hpp"


namespace re {

    SceneInspector::SceneInspector(std::shared_ptr<Scene> scene) : scene(std::move(scene)) {

    }

    void SceneInspector::drawScene() {
        popupContext();

        for (auto& entity : scene->getEntities()) {
            showEntityTree(entity);
        }
    }

    void SceneInspector::showEntityTree(const std::shared_ptr<Entity>& entity) {
        bool nodeOpen = ImGui::TreeNodeEx(entity->getName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::Selectable("Add Entity")) { addEntity(entity); };
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

    void SceneInspector::popupContext() {
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Add Entity")) { addEntity(); };
            ImGui::EndPopup();
        }
    }

    std::shared_ptr<Entity> SceneInspector::getSelectedEntity() {
        return selectedEntity;
    }

    void SceneInspector::addEntity(const std::shared_ptr<Entity>& entity) {
        if (entity) {
            entity->addChild("Entity");
        } else {
            scene->addEntity("Entity");
        }
    }

} // namespace re
