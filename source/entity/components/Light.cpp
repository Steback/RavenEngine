#include "Light.hpp"


namespace re {

    /**
     *
     * @param color Light color (RGB)
     * @param ambient Ambient intensity
     */
    Light::Light(const vec3& color, float ambient)
            : Component(nullptr), color(color), ambient(ambient) {

    }

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
            {"color", {
                {"r", color.x},
                {"g", color.y},
                {"b", color.z}
            }},
            {"ambient", ambient}
        };
    }

    void Light::serialize(json &component) {
        auto col = component["color"];
        color = {col["r"].get<float>(), col["g"].get<float>(), col["b"].get<float>()};
        ambient = component["ambient"].get<float>();
    }

} // namespace re
