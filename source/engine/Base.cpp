#include "Base.hpp"

#include "GLFW/glfw3.h"

#include "engine/render/pipelines/Shader.hpp"


namespace re {

    // TODO: Refactored Base class and add Doxygen comments
    Base::Base(const std::string& appName) {
        // Default search paths
        files::FilesManager::setRootPath();
        // TODO: Change logs path to use OS specific config paths
        files::addPath("logs", true);
        files::addPath("assets");
        files::addPath("shaders");
        files::addPath("data");

        // TODO: Set more logs files
        log::addFile(log::DEFAULT_FILE_NAME);

        config = Config("config.json");
        config.load();

        Time::singleton = new Time();
        input::InputSystem::singleton = new input::InputSystem();
        renderer = std::make_unique<Renderer>(appName, config);
        AssetsManager::singleton = new AssetsManager(renderer->getDevice());
        jobs::JobSystem::singleton = new jobs::JobSystem();

        cli::CliConfig::singleton = new cli::CliConfig(appName);
        cli::addFlag("compile-shaders", "Compile shaders at moment to create its module");
    }

    Base::~Base() {
        delete cli::CliConfig::singleton;
        delete jobs::JobSystem::singleton;
        delete AssetsManager::singleton;
        delete input::InputSystem::singleton;
        delete Time::singleton;
    }

    void Base::loop() {
        while (renderer->isWindowOpen()) {
            glfwPollEvents();
            update();
            render();
        }

        renderer->waitDeviceIde();
    }

    void Base::run() {
        cli::checkFlags();
        loop();
    }

    void Base::update() {
        Time::getInstance()->update();

        if (renderSystem) renderSystem->update(renderer->getAspectRatio());

        scene->update();
        onUpdate();
    }

    void Base::render() {
        auto commandBuffer = renderer->beginFrame();
        renderer->beginSwapChainRenderPass(commandBuffer);

        // TODO: Change this
        if (scene->loaded() && renderSystem) {
            renderSystem->renderScene(commandBuffer, scene);
        }

        renderer->newImGuiFrame();

        onDrawImGui();

        renderer->renderImGui(commandBuffer);

        renderer->endSwapChainRenderPass(commandBuffer);
        renderer->endFrame();
    }

    void Base::loadScene(const std::string &fileName) {
        scene = std::make_shared<re::Scene>(fileName);

        jobs::submit([=, this](){
            scene->load();
            AssetsManager::getInstance()->setupDescriptorsPool(renderer->getImageCount());
            renderSystem = std::make_unique<RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model");
            onLoadScene();
            scene->setLoaded(true);
        });
    }

} // namespace re