#include "Light.hpp"

#include "nameof.hpp"


namespace re {

    /**
     *
     * @param color Light color (RGB)
     * @param ambient Ambient intensity
     * @param owner Valid pointer to Entity
     */
    Light::Light(const vec3 &color, float ambient, Entity *owner)
            : Component(owner), color(color), ambient(ambient) {

    }

    /**
     *
     * @param component Component data serialized in JSON
     * @param owner Valid pointer to Entity
     */
    Light::Light(json &component, Entity *owner) : Component(owner) {
        serialize(component);
    }

    Light::~Light() = default;

    json Light::serialize() {
        return {
            {std::string(NAMEOF(color)), {
                color.x,
                color.y,
                color.z
            }},
            {std::string(NAMEOF(ambient)), ambient}
        };
    }

    void Light::serialize(json &component) {
        color = Vector3(component[std::string(NAMEOF(color))].get<std::array<float, 3>>().data());
        ambient = component[std::string(NAMEOF(ambient))].get<float>();
    }

} // namespace re
