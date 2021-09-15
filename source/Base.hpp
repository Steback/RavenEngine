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
#include "config/Cli.hpp"
#include "jobSystem/JobSystem.hpp"


namespace re {

    /**
     * @brief Base class. this have all the engine's core functionalities
     */
    class Base : NonCopyable {
    public:
        /**
         * @param appName Application/Game name
         */
        explicit Base(const std::string& appName);

        ~Base() override;

        /**
         * @brief Update function for App/Game class. Will executed during the loop, before render.
         */
        virtual void onUpdate() {}

        /**
         * @brief Use for Application to draw ImGui stuff when the engine render it.
         */
        virtual void onDrawImGui() {}

        /**
         * @brief Used for application when scene are loaded.
         */
        virtual void onLoadScene() {};

        /**
         * @brief Main loop, execute all update and render functionalities
         */
        void loop();

        /**
         * @brief
         */
        void run();

        /**
         * @brief Load scene from json file
         */
        void loadScene(const std::string& fileName);

    protected:
        /**
         * @brief Main update function. Can be overwritten
         */
        virtual void update();

        /**
         * @brief Main render function. Can be overwritten
         */
        virtual void render();

    protected:
        Config config;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<RenderSystem> renderSystem;
        std::shared_ptr<Scene> scene;
        std::shared_ptr<JobSystem> jobSystem;
    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
