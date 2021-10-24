#include "ElementInspector.hpp"

#include "imgui.h"
#include "nameof.hpp"

#include "engine/entity/Entity.hpp"
#include "engine/entity/components/Transform.hpp"


namespace re {

    // TODO: Change for use T element instead hardcode the type
    ElementInspector::ElementInspector() = default;

    void ElementInspector::setEntity(std::shared_ptr<Entity> newEntity) {
        element = std::move(newEntity);
    }

    void ElementInspector::showEntityData() {
        if (element) {
            ImGui::Text("ID: %i", element->getId());
            elementName();
            componentsInfo();
        }
    }

    void ElementInspector::elementName() {
        std::array<char, 30> buffer{};
        std::copy(element->getName().begin(), element->getName().end(), buffer.begin());
        ImGui::InputText("Name", buffer.data(), buffer.size());
        element->setName(buffer.data());
    }

    void ElementInspector::componentsInfo() {
        if (ImGui::CollapsingHeader(NAMEOF_SHORT_TYPE(Transform).data())) {
            auto& transform = element->getComponent<Transform>();

            ImGui::InputFloat3("Position", transform.position.values);

            vec3 rotate = transform.rotation.getAngles();
            vec3 angles = {rotate.z, rotate.x, rotate.y};
            ImGui::InputFloat3("Rotation", angles.values);
            transform.rotation = quat(Vector3{angles.y, angles.z, angles.x});

            ImGui::InputFloat3("Scale", transform.scale.values);
        }
    }

} // namespace re
