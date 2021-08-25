#ifndef RAVENENGINE_COMPONENT_HPP
#define RAVENENGINE_COMPONENT_HPP


namespace re {

    class Entity;

    class Component {
        friend Entity;

    protected:
        /**
         *
         * @param owner Pointer to entity owner of component
         */
        explicit Component(Entity* owner);

    protected:
        Entity* owner;
    };

} // namespace re


#endif //RAVENENGINE_COMPONENT_HPP
