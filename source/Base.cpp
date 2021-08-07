#include "Base.hpp"

#include "render/Window.hpp"


namespace re {

    Base::Base(const eastl::string& appName) {
        window = eastl::make_shared<Window>(appName, 800, 600);
    }

    Base::~Base() = default;

    void Base::loop() {
        while (window->isOpen()) {
            glfwPollEvents();
            update();
        }
    }

    void Base::run() {
        start();
        loop();
    }

} // namespace re