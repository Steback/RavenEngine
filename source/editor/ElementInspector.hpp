#ifndef RAVENENGINE_ELEMENTINSPECTOR_HPP
#define RAVENENGINE_ELEMENTINSPECTOR_HPP


#include <memory>


namespace re {

    class Entity;

    class ElementInspector {
    public:
        ElementInspector();

        void setEntity(std::shared_ptr<Entity> newEntity);

        void showEntityData();

    private:
        void elementName();

        void componentsInfo();

        std::shared_ptr<Entity> element;
    };

} // namespace re

#endif //RAVENENGINE_ELEMENTINSPECTOR_HPP
