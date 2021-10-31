#include "Engine.hpp"


namespace re {

    Engine::Engine(const std::string& appName, Config& config) : config(config) {
        input::InputSystem::singleton = new input::InputSystem();

        files::FilesManager::setRootPath();
        // TODO: Change logs path to use OS specific config paths
        files::addPath("logs", true);
        files::addPath("assets");
        files::addPath("shaders");
        files::addPath("data");

        // TODO: Set more logs files
        log::LogsManager::cleanLogsFiles();
        log::addFile(log::DEFAULT_FILE_NAME);

        cli::CliConfig::singleton = new cli::CliConfig(appName);
        cli::addFlag("--compile-shaders", "Compile shaders at moment to create its module");

        config = Config("config.json");
        config.load();
    }

    Engine::~Engine() {
        delete AssetsManager::singleton;
        delete jobs::JobSystem::singleton;
        delete Time::singleton;
        delete cli::CliConfig::singleton;
        delete input::InputSystem::singleton;
    }

    void Engine::setup() {
        // TODO: Change config
        Time::singleton = new Time();
        jobs::JobSystem::singleton = new jobs::JobSystem();
        renderer = std::make_unique<Renderer>("", config);
        AssetsManager::singleton = new AssetsManager(renderer->getDevice());
    }

    // TODO: Find a better solution for this callbacks in update and render
    void Engine::loop(const std::function<void()>& updateCallback, const std::function<void()>& renderUiCallback) {
        while (renderer->isWindowOpen()) {
            glfwPollEvents();
            update(updateCallback);
            render(renderUiCallback);
        }

        renderer->waitDeviceIde();
    }

    void Engine::update(const std::function<void()>& updateCallback) {
        Time::getInstance()->update();

        if (renderSystem) renderSystem->update(renderer->getAspectRatio());

        scene->update();
        updateCallback();
    }

    void Engine::render(const std::function<void()>& renderUiCallback) {
        auto commandBuffer = renderer->beginFrame();
        renderer->beginSwapChainRenderPass(commandBuffer);

        // TODO: Change this
        if (scene->loaded() && renderSystem) {
            renderSystem->renderScene(commandBuffer, scene);
        }

        renderer->newImGuiFrame();

        renderUiCallback();

        renderer->renderImGui(commandBuffer);

        renderer->endSwapChainRenderPass(commandBuffer);
        renderer->endFrame();
    }

    void Engine::setScene(std::shared_ptr<Scene> newScene) {
        scene = std::move(newScene);
    }

    std::shared_ptr<Scene> Engine::getScene() {
        return scene;
    }

    Renderer &Engine::getRenderer() {
        return *renderer;
    }

    void Engine::setRenderSystem(std::unique_ptr<RenderSystem> newRenderSystem) {
        renderSystem = std::move(newRenderSystem);
    }

} // namespace re
