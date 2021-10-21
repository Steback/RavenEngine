#include "Component.hpp"


namespace re {

    Component::Component(Entity *owner) : owner(owner) {

    }

    /**
     *
     * @return JSON object with component serialized data
     */
    json Component::serialize() {
        return {};
    }

    /**
     *
     * @param component Component data serialized in JSON
     */
    void Component::serialize(json &component) {

    }

} // namespace re
