#ifndef RAVENENGINE_EDITOR_SCENEINSPECTOR_HPP
#define RAVENENGINE_EDITOR_SCENEINSPECTOR_HPP


#include <memory>


namespace re {

    class Scene;

    class SceneInspector {
    public:
        explicit SceneInspector(std::shared_ptr<Scene> scene);

        void drawScene();

    private:
        std::shared_ptr<Scene> scene;
    };

} // namespace re


#endif //RAVENENGINE_EDITOR_SCENEINSPECTOR_HPP
