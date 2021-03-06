#ifndef RAVENENGINE_COMPONENT_HPP
#define RAVENENGINE_COMPONENT_HPP


#include "engine/external/Json.hpp"


namespace re {

    class Entity;

    // TODO: Implement Constructor form serialized
    class Component {
        friend Entity;

    public:
        explicit Component(Entity* owner);

        virtual json serialize();

        virtual void serialize(json& component);

    protected:
        Entity* owner;
    };

} // namespace re


#endif //RAVENENGINE_COMPONENT_HPP
