#ifndef RAVENENGINE_ENTITY_HPP
#define RAVENENGINE_ENTITY_HPP


#include "scene/Scene.hpp"

#include "external/Json.hpp"

#include "utils/NonCopyable.hpp"


namespace re {

    /**
     * @brief Entity class
     */
    class Entity : NonCopyable {
    public:
        /**
         *
         * @param name Entity name
         * @param id Valid entity id
         * @param scene Pointer to scene that owns the entity
         */
        Entity(std::string  name, id_t id, Scene* scene);

        /**
         * @brief Construct Entity from JSON
         * @param entity json Object
         * @param id Valid entity id
         * @param scene
         */
        Entity(json& entity, id_t id, Scene* scene);

        ~Entity() override;

        /**
         * @brief Add a new Component to Entity
         * @tparam T Component type
         * @tparam Args Constructor args types of Component
         * @param args Constructor args of Component
         * @return Reference of Component
         */
        template<typename T, typename ...Args>
        T& addComponent(Args&& ...args);

        /**
         *
         * @tparam T Component type
         * @return Reference of Component
         */
        template<typename T>
        T& getComponent();

        /**
         *
         * @tparam T Component type
         */
        template<typename T>
        bool hasComponent();

        [[nodiscard]] id_t getId() const;

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        /**
         * @brief Serialize Entity to JSON
         * @param data
         */
        json serialize();

        /**
         * @brief Serialize Entity from JSON
         * @param entity json object
         */
        void serialize(json& entity);

    private:
        Scene* scene;
        id_t id;
        std::string name;
    };

    template<typename T, typename ...Args>
    T &Entity::addComponent(Args&& ...args) {
        return scene->registry.emplace<T>(id, std::forward<Args>(args)..., this);
    }

    template<typename T>
    T &Entity::getComponent() {
        return scene->registry.get<T>(id);
    }

    template<typename T>
    bool Entity::hasComponent() {
        return scene->registry.all_of<T>(id);
    }

} // namespace re


#endif //RAVENENGINE_ENTITY_HPP
