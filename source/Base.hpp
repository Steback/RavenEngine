#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include <memory>

#include "utils/NonCopyable.hpp"
#include "logs/Logs.hpp"
#include "files/FilesManager.hpp"


namespace re {

    class Renderer;

    class Base : NonCopyable {
    public:
        explicit Base(const std::string& appName);

        ~Base() override;

        virtual void start() = 0;

        virtual void update() = 0;

        void loop();

        void run();

    private:
        std::unique_ptr<Renderer> renderer;
    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
