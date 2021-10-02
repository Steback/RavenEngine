#ifndef RAVENENGINE_INPUT_HPP
#define RAVENENGINE_INPUT_HPP


#include "InputSystem.hpp"

namespace re::input {

    inline bool getKey(Key key, bool once = false) {
        if (once) {
            bool value = InputSystem::getInstance()->getKey(key);
            InputSystem::getInstance()->setKey(key, State::RELEASE);
            return value;
        } else {
            return InputSystem::getInstance()->getKey(key);
        }
    }

    inline void setKey(Key key, State state) {
        InputSystem::getInstance()->setKey(key, state);
    }

    inline bool getMouseButton(MouseButton button, bool once = false) {
        if (once) {
            bool value = InputSystem::getInstance()->getMouseButton(button);
            InputSystem::getInstance()->setMouseButton(key, State::RELEASE);
            return value;
        } else {
            return InputSystem::getInstance()->getMouseButton(button);
        }
    }

    inline voud setMouseButton(MouseButton button, State state) {
        InputSystem::getInstance()->setMouseButton(button, state);
    }

} // namespace re::input


#endif //RAVENENGINE_INPUT_HPP
