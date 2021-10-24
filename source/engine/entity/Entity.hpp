#ifndef RAVENENGINE_ENTITY_HPP
#define RAVENENGINE_ENTITY_HPP


#include "engine/scene/Scene.hpp"

#include "engine/external/Json.hpp"

#include "engine/utils/NonCopyable.hpp"


namespace re {

    class Entity : NonCopyable {
    public:
        Entity(std::string  name, id_t id, Scene* scene);

        Entity(json& entity, id_t id, Scene* scene);

        ~Entity() override;

        template<typename T, typename ...Args>
        inline T& addComponent(Args&& ...args);

        template<typename T>
        inline T& getComponent();

        template<typename T>
        inline bool hasComponent();

        [[nodiscard]] id_t getId() const;

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        json serialize();

        void serialize(json& entity);

        std::shared_ptr<Entity> addChild(const std::string& childName);

        std::shared_ptr<Entity> getChild(const std::string& childName);

        void setParent(std::shared_ptr<Entity> newParent);

        std::shared_ptr<Entity> getParent();

        std::vector<std::shared_ptr<Entity>>& getChildren();

    private:
        Scene* scene;
        id_t id;
        std::string name;
        std::shared_ptr<Entity> parent;
        std::vector<std::shared_ptr<Entity>> children;
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
