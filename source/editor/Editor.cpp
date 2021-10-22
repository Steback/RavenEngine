#include "Editor.hpp"

#include "engine/entity/Entity.hpp"


namespace re {

    Editor::Editor(CLI::App &app) : Base("Editor", app) {
        scene = std::make_shared<Scene>();
        camera = scene->addEntity("Editor Camera");
        camera->addComponent<Transform>(Vector3{}, Vector3{1.0f}, Vector3{});
        camera->addComponent<Camera>(Math::deg2rad(45.0f), 0.01f, 100.0f, cameraTarget);

        windowSize = renderer->getWindowSize();
        scenePanelPos = {0.0f, mainMenuHeight};
        scenePanelSize = {defaultPanelWidth, windowSize.y * 0.6f};
        elementPanelPos = {windowSize.x - defaultPanelWidth, 22.0f};
        elementPanelSize = {defaultPanelWidth, windowSize.y};
    }

    void Editor::onUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        windowSize = {io.DisplaySize.x, io.DisplaySize.y};
    }

    void Editor::onDrawImGui() {
        ImGui::ShowDemoWindow();
        mainMenuBar();
        scenePanel();
        elementPanel();
        miscPanel();
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

                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();
    }

    void Editor::scenePanel() {
        ImGui::SetNextWindowPos({scenePanelPos.x, scenePanelPos.y});
        ImGui::SetNextWindowSize({scenePanelSize.x, scenePanelSize.y});
        ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
        {
            ImGui::BeginTabBar("Scene Info", ImGuiTabBarFlags_None);
            {
                if (ImGui::BeginTabItem("Scene")) {

                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    void Editor::elementPanel() {
        ImGui::SetNextWindowPos({elementPanelPos.x, elementPanelPos.y});
        ImGui::SetNextWindowSize({elementPanelSize.x, elementPanelSize.y});
        ImGui::Begin("Element", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
        {

        }
        ImGui::End();
    }

    void Editor::miscPanel() {
        ImGui::SetNextWindowPos({scenePanelPos.x, scenePanelSize.y + mainMenuHeight});
        ImGui::SetNextWindowSize({windowSize.x - elementPanelSize.x, windowSize.y - scenePanelSize.y});
        ImGui::Begin("Misc", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
        {

        }
        ImGui::End();
    }


}
