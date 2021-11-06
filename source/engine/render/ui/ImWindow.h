#ifndef RAVENENGINE_IMWINDOW_H
#define RAVENENGINE_IMWINDOW_H


#include <string>
#include <functional>

#include "imgui.h"

#include "engine/math/Vector2.hpp"


namespace re::ui {

    class ImWindow {
    public:
        ImWindow() = default;

        ImWindow(std::string  name, const Vector2& size, const Vector2& pos, uint32_t flags = ImGuiWindowFlags_None);

        void draw(const std::function<void()>& call, bool allowClose = false);

        Vector2 size;
        Vector2 position;
        std::string name;
        uint16_t flags{ImGuiWindowFlags_None};
        bool open{false};
    };

} // namespace re

#endif //RAVENENGINE_IMWINDOW_H
