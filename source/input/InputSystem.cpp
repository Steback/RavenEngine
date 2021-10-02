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
    void InputSystem::setKey(int key, State state) {
        keys[key] = state;
    }

    /**
     * @note Just LEFT, RIGHT and MIDDLE button work for now
     * @param button Button key to query
     * @return Current state of button
     */
    bool InputSystem::getMouseButton(MouseButton button) {
        return mouseButtons[button];
    }

    /**
     * @note Just LEFT, RIGHT and MIDDLE button work for now
     * @param button Button key to query
     * @param state New state to set
     */
    void InputSystem::setMouseButton(MouseButton button, State state) {
        mouseButtons[button] = state;
    }

    void InputSystem::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == State::PRESS) {
            getInstance()->setKey(key, State::PRESS);
        } else if (action == State::RELEASE) {
            getInstance()->setKey(key, State::RELEASE);
        }

    }

    void InputSystem::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        // TODO: Temporally just 3 mouse buttons
        if (button > 2) return;

        if (action == State::PRESS) {
            getInstance()->mouseButtons[button] = State::PRESS;
        } else if (action == State::RELEASE) {
            getInstance()->mouseButtons[button] = State::RELEASE;
        }
    }

} // namespace re::input
