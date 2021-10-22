#ifndef RAVENENGINE_EDITOR_HPP
#define RAVENENGINE_EDITOR_HPP


#include "engine/Base.hpp"
#include "SceneInspector.hpp"


namespace re {

    class Editor : public Base {
    public:
        explicit Editor(CLI::App& app);

        void onDrawImGui() override;

        void onUpdate() override;

    private:
        void mainMenuBar();

        void scenePanel();

        void elementPanel();

        void miscPanel();

        std::shared_ptr<Entity> camera;
        std::unique_ptr<SceneInspector> sceneInspector;
        float mainMenuHeight{22.0f};
        float defaultPanelWidth{300.0f};
        vec3 cameraTarget;
        vec2 windowSize;
        vec2 scenePanelSize;
        vec2 elementPanelSize;
        vec2 scenePanelPos;
        vec2 elementPanelPos;
        bool saveSceneOpen = false;
    };

}

#endif //RAVENENGINE_EDITOR_HPP
