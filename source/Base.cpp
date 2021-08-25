#include "Base.hpp"

#include "GLFW/glfw3.h"


namespace re {

    Base::Base(const std::string& appName) {
        config = Config("config.json");
        config.load();

        renderer = std::make_unique<Renderer>(appName, config);
        assetsManager = std::make_unique<AssetsManager>(renderer->getDevice());
        scene = std::make_unique<Scene>();
        renderSystem = std::make_unique<RenderSystem>(renderer->getDevice(), renderer->getRenderPass(), "model", std::vector<VkPushConstantRange>{});
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
        onUpdate();
    }

    void Base::render() {
        auto commandBuffer = renderer->beginFrame();
        renderer->beginSwapChainRenderPass(commandBuffer);

        renderSystem->renderEntities(commandBuffer);

        renderer->endSwapChainRenderPass(commandBuffer);
        renderer->endFrame();
    }

} // namespace re