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
         *
         * Pure virtual method, will be implemented by the Application/Game class.
         */
        virtual void onUpdate() = 0;

        /**
         * @brief Use for Application to draw ImGui stuff when the engine render it.
         *
         * Pure virtual method, will be implemented by the Application/Game class.
         */
        virtual void onDrawImGui()  = 0;

        /**
         * @brief Main loop, execute all update and render functionalities
         */
        void loop();

        /**
         * @brief
         */
        void run();

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
        std::unique_ptr<AssetsManager> assetsManager;
        std::unique_ptr<RenderSystem> renderSystem;
        std::unique_ptr<Scene> scene;
    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
