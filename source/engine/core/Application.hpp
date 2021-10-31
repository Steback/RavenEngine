#ifndef RAVENENGINE_APPLICATION_HPP
#define RAVENENGINE_APPLICATION_HPP


#include <memory>

#include "Engine.hpp"


namespace re {

    class Application : NonCopyable {
    public:
        explicit Application(const std::string& appName);

        ~Application() override;

        void run();

    protected:
        virtual void setup();

        virtual void update();

        virtual void drawImGui();

    protected:
        Config config;
        std::unique_ptr<Engine> engine;
    };

} // namespace re


#endif //RAVENENGINE_APPLICATION_HPP
