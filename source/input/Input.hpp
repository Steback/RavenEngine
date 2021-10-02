#ifndef RAVENENGINE_INPUT_HPP
#define RAVENENGINE_INPUT_HPP


#include "InputSystem.hpp"

namespace re::input {

    inline bool getKey(int key, bool once = false) {
        if (once) {
            bool value = InputSystem::getInstance()->getKey(key);
            InputSystem::getInstance()->setKey(key, State::RELEASE);
            return value;
        } else {
            return InputSystem::getInstance()->getKey(key);
        }
    }

    inline void setKey(int key, State state) {
        InputSystem::getInstance()->setKey(key, state);
    }

} // namespace re::input


#endif //RAVENENGINE_INPUT_HPP
