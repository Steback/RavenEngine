#include "Editor.hpp"

#include "imgui.h"

#include "engine/entity/Entity.hpp"


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
        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Save")) {}
                if (ImGui::MenuItem("Save as")) {}
                if (ImGui::MenuItem("Open")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
#ifdef RE_DEBUG
                if (ImGui::MenuItem("ImGui Demo")) { imguiDemoWindow = true; }
#endif
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();
    }

    void Editor::scenePanel() {
        sceneWindow.draw([&, this]{
            scenePanelSize.x = ImGui::GetWindowSize().x;

            ImGui::BeginTabBar("Scene Info", ImGuiTabBarFlags_None);
            {
                if (ImGui::BeginTabItem("Scene")) {
                    sceneInspector->drawScene();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
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
