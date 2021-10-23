#ifndef RAVENENGINE_EDITOR_SCENEINSPECTOR_HPP
#define RAVENENGINE_EDITOR_SCENEINSPECTOR_HPP


#include <memory>


namespace re {

    class Scene;
    class Entity;

    class SceneInspector {
    public:
        explicit SceneInspector(std::shared_ptr<Scene> scene);

        void drawScene();

        std::shared_ptr<Entity> getSelectedEntity();

    private:
        void showEntityTree(const std::shared_ptr<Entity>& entity);

        void popupContext(const std::shared_ptr<Entity>& entity = nullptr);

        void addEntity(const std::shared_ptr<Entity>& entity = nullptr);

        std::shared_ptr<Scene> scene;
        std::shared_ptr<Entity> selectedEntity;
    };

} // namespace re


#endif //RAVENENGINE_EDITOR_SCENEINSPECTOR_HPP
