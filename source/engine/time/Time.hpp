#ifndef RAVENENGINE_TIME_HPP
#define RAVENENGINE_TIME_HPP


#include <chrono>


namespace re {

    class Time {
        friend class Engine;

        Time();

    public:
        static Time* getInstance();

        void update();

        [[nodiscard]] float getDeltaTime() const;

    private:
        static Time* singleton;
        float deltaTime;
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> currentTime;
    };

    namespace time {

        inline float deltaTime() {
            return Time::getInstance()->getDeltaTime();
        }

    } // namespace time

} // namespace re


#endif //RAVENENGINE_TIME_HPP
