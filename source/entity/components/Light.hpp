#ifndef RAVENENGINE_LIGHT_HPP
#define RAVENENGINE_LIGHT_HPP


#include "Component.hpp"
#include "math/Vector3.hpp"


namespace re {

    class Light : public Component {
    public:
        struct Ubo {
            alignas(16) vec3 position{0.0f};
            alignas(16) vec3 color{1.0f};
            float ambient = 0.2f;
        };

        Light(const vec3& color, float ambient);

        Light(const vec3& color, float ambient, Entity* owner);

        Light(json& component, Entity* owner);

        ~Light();

        json serialize() override;

        void serialize(json &component) override;

    public:
        vec3 color{1.0f};
        float ambient = 0.1f;
    };

} // namespace re


#endif //RAVENENGINE_LIGHT_HPP
