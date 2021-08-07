#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include "EASTL/string.h"
#include "EASTL/unique_ptr.h"

#include "utils/NonCopyable.hpp"
#include "logs/Logs.hpp"
#include "files/FilesManager.hpp"


namespace re {

    class Renderer;

    class Base : NonCopyable {
    public:
        explicit Base(const eastl::string& appName);

        ~Base() override;

        virtual void start() = 0;

        virtual void update() = 0;

        void loop();

        void run();

    private:
        eastl::unique_ptr<Renderer> renderer;
    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
