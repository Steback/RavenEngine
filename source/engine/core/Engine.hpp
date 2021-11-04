#ifndef RAVENENGINE_ENGINE_HPP
#define RAVENENGINE_ENGINE_HPP

#include <functional>

#include "NonCopyable.hpp"
#include "Utils.hpp"
#include "engine/config/Config.hpp"
#include "engine/time/Time.hpp"
#include "engine/input/Input.hpp"
#include "engine/assets/AssetsManager.hpp"
#include "engine/render/Renderer.hpp"
#include "engine/jobSystem/JobSystem.hpp"
#include "engine/render/RenderSystem.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/config/Config.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/config/CliConfig.hpp"


namespace re {

    class Engine : NonCopyable {
    public:
        Engine(const std::string& appName, Config& config);

        ~Engine() override;

        void setup();

        void loop(const std::function<void()>& updateCallback = nullptr, const std::function<void()>& renderUiCallback = nullptr);

        void update(const std::function<void()>& updateCallback);

        void render(const std::function<void()>& renderUiCallback);

        void setScene(std::shared_ptr<Scene> newScene);

        std::shared_ptr<Scene> getScene();

        Renderer& getRenderer();

        void setRenderSystem(std::unique_ptr<RenderSystem> newRenderSystem);

    private:
        Config& config;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<RenderSystem> renderSystem;
        std::shared_ptr<Scene> scene;
    };

} // namespace re

#endif //RAVENENGINE_ENGINE_HPP
