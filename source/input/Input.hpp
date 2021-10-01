#ifndef RAVENENGINE_INPUT_HPP
#define RAVENENGINE_INPUT_HPP


#include "InputSystem.hpp"

namespace re::input {

    inline bool getKey(int key) {
        return InputSystem::getInstance()->getKey(key);
    }

    inline void setKey(int key, KeyState state) {
        InputSystem::getInstance()->setKey(key, state);
    }

} // namespace re::input


#endif //RAVENENGINE_INPUT_HPP
