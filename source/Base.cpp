#include "Base.hpp"

#include "GLFW/glfw3.h"

#include "render/pipelines/Shader.hpp"


namespace re {

    Base::Base(const std::string& appName) {
        FilesManager::singleton = new files::FilesManager();
        files::addPath("logs", true);
        files::addPath("assets");
        files::addPath("shaders");
        files::addPath("data");

        re::Logger::setup();

        config = Config("config.json");
        config.load();

        renderer = std::make_unique<Renderer>(appName, config);
        AssetsManager::singleton = new AssetsManager(renderer->getDevice());
        JobSystem::singleton = new JobSystem();
    }

    Base::~Base() {
        delete JobSystem::singleton;
        delete AssetsManager::singleton;
        delete FilesManager::singleton;
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

        JobSystem::getInstance()->submit([=, this](){
            scene->load();
            AssetsManager::getInstance()->setupDescriptorsPool(renderer->getImageCount());
            renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model");
            onLoadScene();
            scene->setLoaded(true);
        });
    }

} // namespace re