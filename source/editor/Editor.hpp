#ifndef RAVENENGINE_EDITOR_HPP
#define RAVENENGINE_EDITOR_HPP


#include "engine/core/Application.hpp"
#include "SceneInspector.hpp"
#include "ElementInspector.hpp"


namespace re {

    class Editor : public Application {
    public:
        explicit Editor();

    private:
        void setup() override;

        void drawImGui() override;

        void update() override;

        void mainMenuBar();

        void scenePanel();

        void elementPanel();

        void miscPanel();

        std::shared_ptr<Entity> camera;
        std::unique_ptr<SceneInspector> sceneInspector;
        std::unique_ptr<ElementInspector> elementInspector;
        float mainMenuHeight{22.0f};
        float defaultPanelWidth{300.0f};
        vec3 cameraTarget;
        vec2 windowSize;
        vec2 scenePanelSize;
        vec2 elementPanelSize;
        vec2 scenePanelPos;
        vec2 elementPanelPos;
#ifdef RE_DEBUG
        bool imguiDemoWindow = false;
#endif
    };

}

#endif //RAVENENGINE_EDITOR_HPP
