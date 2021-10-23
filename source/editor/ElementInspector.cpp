#include "ElementInspector.hpp"

#include "imgui.h"

#include "engine/entity/Entity.hpp"


namespace re {

    // TODO: Change for use T element instead hardcode the type
    ElementInspector::ElementInspector() = default;

    void ElementInspector::setEntity(std::shared_ptr<Entity> newEntity) {
        element = std::move(newEntity);
    }

    void ElementInspector::showEntityData() {
        if (element) {
            elementName();
        }
    }

    void ElementInspector::elementName() {
        std::array<char, 30> buffer{};
        std::copy(element->getName().begin(), element->getName().end(), buffer.begin());
        ImGui::InputText("Name", buffer.data(), buffer.size());
        element->setName(buffer.data());
    }

} // namespace re
