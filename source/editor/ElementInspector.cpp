#include "ElementInspector.hpp"

#include "nameof.hpp"

#include "engine/entity/Entity.hpp"
#include "engine/entity/components/Transform.hpp"
#include "engine/render/ui/ImElements.hpp"


namespace re {

    // TODO: Change for use T element instead hardcode the type
    ElementInspector::ElementInspector() = default;

    void ElementInspector::setEntity(std::shared_ptr<Entity> newEntity) {
        element = std::move(newEntity);
    }

    void ElementInspector::showEntityData() {
        if (element) {
            ui::imText("ID {}", element->getId());
            element->setName(ui::imInputText("Name", element->getName()));
            componentsInfo();
        }
    }

    void ElementInspector::componentsInfo() {
        ui::imCollapsingHeader(NAMEOF_SHORT_TYPE(Transform).data(), [&, this]{
            auto& transform = element->getComponent<Transform>();

            ui::imInputVec3("Position", transform.position);

            vec3 rotate = transform.rotation.getAngles();
            vec3 angles = {rotate.z, rotate.x, rotate.y};
            ui::imInputVec3("Rotation", angles);
            transform.rotation = quat(Vector3{angles.y, angles.z, angles.x});

            ui::imInputVec3("Scale", transform.scale);
        });
    }

} // namespace re
