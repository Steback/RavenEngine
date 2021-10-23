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
        if (ImGui::TreeNode(entity->getName().c_str())) {
            for (auto& child : entity->getChildren()) {
                showEntityTree(child);
            }
        }
    }

    void SceneInspector::popupContext() {
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Add Entity")) { scene->addEntity("Entity"); };
            ImGui::EndPopup();
        }
    }

} // namespace re
