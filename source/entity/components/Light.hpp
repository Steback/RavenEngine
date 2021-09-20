#ifndef RAVENENGINE_LIGHT_HPP
#define RAVENENGINE_LIGHT_HPP


#include "Component.hpp"
#include "math/Vector3.hpp"


namespace re {

    /**
     * @brief Light Component
     */
    class Light : public Component {
    public:
        struct Ubo {
            vec3 color{1.0f};
            float ambient = 0.1f;
        };

        /**
         *
         * @param color
         * @param ambient
         */
        Light(const vec3& color, float ambient);

        /**
         *
         * @param color
         * @param ambient
         * @param owner Valid pointer to entity that owns this component
         */
        Light(const vec3& color, float ambient, Entity* owner);

        /**
         * @brief Construct entity from JSON
         * @param component json object
         * @param owner Valid pointer to entity that owns this component
         */
        Light(json& component, Entity* owner);

        ~Light();

        /**
         * @brief Serialize Light to JSON
         */
        json serialize() override;

        /**
         * @brief Serialize Light from JSON
         */
        void serialize(json &component) override;

    public:
        vec3 color{1.0f};
        float ambient = 0.1f;
    };

} // namespace re


#endif //RAVENENGINE_LIGHT_HPP
