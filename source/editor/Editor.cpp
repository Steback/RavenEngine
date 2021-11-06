#include "Editor.hpp"

#include "engine/entity/Entity.hpp"
#include "engine/render/ui/ImElements.hpp"


namespace re {

    Editor::Editor() : Application("Raven Engine Editor") {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        camera = std::make_shared<Entity>("Camera", scene->getRegistry().create(), scene.get());
        camera->addComponent<Transform>(Vector3{}, Vector3{1.0f}, Vector3{});
        camera->addComponent<Camera>(Math::deg2rad(45.0f), 0.01f, 100.0f, cameraTarget);
        scene->setMainCamera(camera);
        engine->setScene(scene);

        sceneInspector = std::make_unique<SceneInspector>(scene);
        elementInspector = std::make_unique<ElementInspector>();
    }

    void Editor::setup() {
        AssetsManager::getInstance()->setupDescriptorsPool(engine->getRenderer().getImageCount());
        std::unique_ptr<RenderSystem> renderSystem = std::make_unique<RenderSystem>(engine->getRenderer().getDevice(), engine->getRenderer().getRenderPass(), "model");
        engine->setRenderSystem(std::move(renderSystem));

        windowSize = engine->getRenderer().getWindowSize();
        scenePanelPos = {0.0f, mainMenuHeight};
        scenePanelSize = {defaultPanelWidth, windowSize.y * 0.6f};
        elementPanelPos = {windowSize.x - defaultPanelWidth, mainMenuHeight};
        elementPanelSize = {defaultPanelWidth, windowSize.y};

        uint32_t flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
        sceneWindow = ui::ImWindow("Scene", scenePanelSize, scenePanelPos, flags);
        elementWindow = ui::ImWindow("Element", elementPanelSize, elementPanelPos, flags);
        miscWindow = ui::ImWindow("Misc", elementPanelSize, elementPanelPos, flags);
    }

    void Editor::update() {
        ImGuiIO& io = ImGui::GetIO();
        windowSize = {io.DisplaySize.x, io.DisplaySize.y};

        sceneWindow.position = {scenePanelPos.x, scenePanelPos.y};
        sceneWindow.size = {scenePanelSize.x, scenePanelSize.y - mainMenuHeight};

        elementWindow.position = {elementPanelPos.x, elementPanelPos.y};
        elementWindow.size = {elementPanelSize.x, elementPanelSize.y - mainMenuHeight};

        miscWindow.position = {scenePanelPos.x, scenePanelSize.y};
        miscWindow.size = {windowSize.x - elementPanelSize.x, windowSize.y - scenePanelSize.y};
    }

    void Editor::drawImGui() {
        mainMenuBar();
        scenePanel();
        elementPanel();
        miscPanel();

#ifdef RE_DEBUG
        if (imguiDemoWindow) ImGui::ShowDemoWindow(&imguiDemoWindow);
#endif
    }

    void Editor::mainMenuBar() {
        ui::imMainMenuBar([&, this]{
            ui::imMenu("File", [&]{
                ui::imMenuItem("New", [&]{});
                ui::imMenuItem("Save", [&]{});
                ui::imMenuItem("Save as", [&]{});
                ui::imMenuItem("Open", [&]{});
            });
            ui::imMenu("Edit", [&]{

            });
            ui::imMenu("Help", [&]{
#ifdef RE_DEBUG
                ui::imMenuItem("ImGui Demo", [&]{ imguiDemoWindow = true; });
#endif
            });
        });
    }

    void Editor::scenePanel() {
        sceneWindow.draw([&, this]{
            scenePanelSize.x = ImGui::GetWindowSize().x;

            ui::imTabBar("Scene Info", [&]{
                ui::imTabItem("Scene", [&]{ sceneInspector->drawScene(); });
            }, ImGuiTabBarFlags_None);
        });
    }

    void Editor::elementPanel() {
        elementWindow.draw([&, this]{
            elementPanelSize.x = ImGui::GetWindowSize().x;
            elementPanelPos.x = windowSize.x - elementPanelSize.x;
            elementInspector->setEntity(sceneInspector->getSelectedEntity());
            elementInspector->showEntityData();
        });
    }

    void Editor::miscPanel() {
        miscWindow.draw([&, this]{
            scenePanelSize.y = ImGui::GetWindowPos().y;
        });
    }
}
