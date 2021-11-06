#include "ImWindow.hpp"

#include <utility>


namespace re::ui {

    ImWindow::ImWindow(std::string  name, const Vector2& size, const Vector2& pos, uint32_t flags)
            : name(std::move(name)), size(size), position(pos), flags(flags) {

    }

    void ImWindow::draw(const std::function<void()>& call, bool allowClose) {
        ImGui::SetNextWindowSize({size.x, size.y});
        ImGui::SetNextWindowPos({position.x, position.y});
        ImGui::Begin(name.c_str(), allowClose ? &open : nullptr, flags);
        call();
        ImGui::End();
    }

} // namespace re::ui
