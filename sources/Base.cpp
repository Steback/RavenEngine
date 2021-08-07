#include "Base.hpp"


namespace re {

    Base::Base() = default;

    Base::~Base() = default;

    void Base::loop() {
        while (true) {
            update()
        }
    }

    void Base::run() {
        start();
        loop();
    }

} // namespace re