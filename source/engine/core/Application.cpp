#include "Application.hpp"


namespace re {

    Application::Application(const std::string& appName) {
        engine = std::make_unique<Engine>(appName, config);
    }

    Application::~Application() = default;

    void Application::run() {
        engine->setup();
        setup();
        engine->loop([&, this]{ update(); }, [&, this]{ drawImGui(); });
    }

    void Application::setup() {

    }

    void Application::update() {

    }

    void Application::drawImGui() {

    }

} // namespace re