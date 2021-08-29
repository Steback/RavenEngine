#ifndef RAVENENGINE_COMPONENT_HPP
#define RAVENENGINE_COMPONENT_HPP


#include "external/Json.hpp"


namespace re {

    class Entity;

    /**
     * @brief Base Component class
     * Any Component that be used in an Entity need to inherit this class
     */
    class Component {
        friend Entity;

    public:
        /**
         * @brief Serialize component to JSON
         */
        virtual json serialize() = 0;

        /**
         * @brief Serialize component from JSON
         */
        virtual void serialize(json& component) = 0;

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
