#include "Base.hpp"

#include "GLFW/glfw3.h"

#include "render/pipelines/Shader.hpp"


namespace re {

    // TODO: Refactored Base class and add Doxygen comments
    Base::Base(const std::string& appName, CLI::App& app) {
        cli::CliOptions::singleton = new cli::CliOptions(app);
        cli::addFlag("compile-shaders", "Compile shaders at moment to create shader module");

        FilesManager::singleton = new files::FilesManager();
        logs::LogsManager::singleton = new logs::LogsManager();

        files::addPath("assets");
        files::addPath("shaders");
        files::addPath("data");

        config = Config("config.json");
        config.load();

        renderer = std::make_unique<Renderer>(appName, config);
        AssetsManager::singleton = new AssetsManager(renderer->getDevice());
        jobs::JobSystem::singleton = new jobs::JobSystem();
    }

    Base::~Base() {
        delete jobs::JobSystem::singleton;
        delete AssetsManager::singleton;
        delete logs::LogsManager::singleton;
        delete FilesManager::singleton;
        delete cli::CliOptions::singleton;
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
        loop();
    }

    void Base::update() {
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
            renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model");
            onLoadScene();
            scene->setLoaded(true);
        });
    }

} // namespace re