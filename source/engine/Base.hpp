#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include <memory>

#include "imgui.h"
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

#include "engine/utils/NonCopyable.hpp"
#include "engine/logs/Logs.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/config/Config.hpp"
#include "engine/render/Renderer.hpp"
#include "engine/assets/AssetsManager.hpp"
#include "engine/render/RenderSystem.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/config/CliOptions.hpp"
#include "engine/jobSystem/JobSystem.hpp"
#include "engine/input/Input.hpp"
#include "engine/time/Time.hpp"


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
