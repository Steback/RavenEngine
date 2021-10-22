#include "Editor.hpp"

#include "engine/entity/Entity.hpp"


namespace re {

    Editor::Editor(CLI::App &app) : Base("Editor", app) {
        scene = std::make_shared<Scene>();
        camera = scene->addEntity("Editor Camera");
        camera->addComponent<Transform>(Vector3{}, Vector3{1.0f}, Vector3{});
        camera->addComponent<Camera>(Math::deg2rad(45.0f), 0.01f, 100.0f, cameraTarget);
    }

}
