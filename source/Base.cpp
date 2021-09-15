#include "Base.hpp"

#include "GLFW/glfw3.h"

#include "render/pipelines/Shader.hpp"


namespace re {

    Base::Base(const std::string& appName) {
        config = Config("config.json");
        config.load();

        renderer = std::make_unique<Renderer>(appName, config);
        assetsManager = std::make_shared<AssetsManager>(renderer->getDevice());
        jobSystem = std::make_shared<JobSystem>();
    }

    Base::~Base() = default;

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
        scene = std::make_shared<re::Scene>(fileName, assetsManager);

        jobSystem->submit([=, this](){
            scene->load();
            assetsManager->setupDescriptorsPool(renderer->getImageCount());
            renderSystem = std::make_unique<re::RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model", assetsManager);
            onLoadScene();
            scene->setLoaded(true);
        });
    }

} // namespace re