#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include "Utils/NonCopyable.hpp"


namespace re {

    class Base : NonCopyable {
    public:
        Base();

        ~Base();

        virtual void start() = 0;

        virtual void update() = 0;

        void loop();

        void run();

    private:

    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
