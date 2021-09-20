#include "Light.hpp"


namespace re {

    Light::Light(const vec3& color, float ambient)
            : Component(nullptr), color(color), ambient(ambient) {

    }

    Light::Light(const vec3 &color, float ambient, Entity *owner)
            : Component(owner), color(color), ambient(ambient) {

    }

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
