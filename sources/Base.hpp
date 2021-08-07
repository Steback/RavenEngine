#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include "EASTL/string.h"

#include "utils/NonCopyable.hpp"


namespace re {

    class Base : NonCopyable {
    public:
        explicit Base(const eastl::string& appName);

        ~Base() override;

        virtual void start() = 0;

        virtual void update() = 0;

        void loop();

        void run();

    private:

    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
