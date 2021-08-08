#include "Base.hpp"

#include "GLFW/glfw3.h"

#include "render/Renderer.hpp"


namespace re {

    Base::Base(const std::string& appName) {
        config = Config("config.json");
        config.load();

        renderer = std::make_unique<Renderer>(appName, config);
    }

    Base::~Base() = default;

    void Base::loop() {
        while (renderer->isWindowOpen()) {
            glfwPollEvents();
            update();
        }
    }

    void Base::run() {
        loop();
    }

} // namespace re