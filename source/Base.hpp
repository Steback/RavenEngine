#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include <memory>

#include "imgui.h"
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

#include "utils/NonCopyable.hpp"
#include "logs/Logs.hpp"
#include "files/FilesManager.hpp"
#include "config/Config.hpp"
#include "render/Renderer.hpp"
#include "assets/AssetsManager.hpp"
#include "render/RenderSystem.hpp"
#include "scene/Scene.hpp"
#include "config/CliOptions.hpp"
#include "jobSystem/JobSystem.hpp"
#include "input/Input.hpp"
#include "time/Time.hpp"


namespace re {

    class Base : NonCopyable {
    public:
        explicit Base(const std::string& appName, CLI::App& app);

        ~Base() override;

        virtual void onUpdate() {}

        virtual void onDrawImGui() {}

        virtual void onLoadScene() {};

        void loop();

        void run();

        void loadScene(const std::string& fileName);

    protected:
        virtual void update();

        virtual void render();

    protected:
        Config config;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<RenderSystem> renderSystem;
        std::shared_ptr<Scene> scene;
    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
