#include "Base.hpp"

#include "GLFW/glfw3.h"

#include "render/Renderer.hpp"


namespace re {

    Base::Base(const std::string& appName) {
        renderer = std::make_unique<Renderer>(appName);
    }

    Base::~Base() = default;

    void Base::loop() {
        while (renderer->isWindowOpen()) {
            glfwPollEvents();
            update();
        }
    }

    void Base::run() {
        start();
        loop();
    }

} // namespace re