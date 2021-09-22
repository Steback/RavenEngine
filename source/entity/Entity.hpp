#ifndef RAVENENGINE_ENTITY_HPP
#define RAVENENGINE_ENTITY_HPP


#include "scene/Scene.hpp"

#include "external/Json.hpp"

#include "utils/NonCopyable.hpp"


namespace re {

    class Entity : NonCopyable {
    public:
        Entity(std::string  name, id_t id, Scene* scene);

        Entity(json& entity, id_t id, Scene* scene);

        ~Entity() override;

        template<typename T, typename ...Args>
        T& addComponent(Args&& ...args);

        template<typename T>
        T& getComponent();

        template<typename T>
        bool hasComponent();

        [[nodiscard]] id_t getId() const;

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        json serialize();

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
