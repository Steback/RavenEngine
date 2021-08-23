#include "Base.hpp"

#include "GLFW/glfw3.h"


namespace re {

    Base::Base(const std::string& appName) {
        config = Config("config.json");
        config.load();

        renderer = std::make_unique<Renderer>(appName, config);
        assetsManager = std::make_unique<AssetsManager>(renderer->getDevice());
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

        renderer->endSwapChainRenderPass(commandBuffer);
        renderer->endFrame();
    }

} // namespace re