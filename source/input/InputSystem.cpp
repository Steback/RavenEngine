#include "InputSystem.hpp"

#include "logs/Logs.hpp"



namespace re::input {

    InputSystem* InputSystem::singleton;

    InputSystem::InputSystem() = default;

    InputSystem::~InputSystem() = default;

    InputSystem *InputSystem::getInstance() {
        return singleton;
    }

    /**
     * @brief Get current state of Key
     * @param key Key code to query
     * @param once [Optional] reset the key value once was query if was pressed. By default is true.
     */
    bool InputSystem::getKey(int key) {
        return keys[key];
    }

    /**
     * @brief Set current state of key
     * @param key Key code to query
     * @param state New state to set
     */
    void InputSystem::setKey(int key, KeyState state) {
        keys[key] = state;
    }

    void InputSystem::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            getInstance()->setKey(key, KeyState::PRESS);
        } else if (action == GLFW_RELEASE) {
            getInstance()->setKey(key, KeyState::RELEASE);
        }

    }

} // namespace re::input
