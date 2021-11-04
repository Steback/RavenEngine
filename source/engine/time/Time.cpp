#include "Time.hpp"


namespace re {

    Time* Time::singleton;

    Time::Time() {
        deltaTime = 0;
//        currentTime = std::chrono::high_resolution_clock::now();
    }

    Time *Time::getInstance() {
        return singleton;
    }

    void Time::update() {
        auto newTime = std::chrono::high_resolution_clock::now();
//        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
//        currentTime = newTime;
    }

    float Time::getDeltaTime() const {
        return deltaTime;
    }


} // namespace re
