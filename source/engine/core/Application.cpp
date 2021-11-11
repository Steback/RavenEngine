#include "Application.hpp"


namespace re {

    Application::Application(const std::string& appName) {
        engine = std::make_unique<Engine>(appName, config, *this);
    }

    Application::~Application() = default;

    void Application::run() {
        engine->loop();
    }

    void Application::setup() {

    }

    void Application::update() {

    }

    void Application::drawImGui() {

    }

} // namespace re