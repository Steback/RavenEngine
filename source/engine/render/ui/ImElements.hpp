#ifndef RAVENENGINE_IMELEMENTS_HPP
#define RAVENENGINE_IMELEMENTS_HPP


#include <functional>
#include <string>
#include <vector>

#include "imgui.h"
#include "fmt/format.h"

#include "engine/math/Vector2.hpp"
#include "engine/math/Vector3.hpp"


namespace re::ui {

#define Call std::function<void()>

    inline void imMainMenuBar(const Call& call) {
        ImGui::BeginMainMenuBar();
        call();
        ImGui::EndMainMenuBar();
    }

    inline void imMenu(const std::string& label, const Call& call, bool enabled = true) {
        if (ImGui::BeginMenu(label.c_str(), enabled)) {
            call();
            ImGui::EndMenu();
        }
    }

    inline void imMenuItem(const std::string& label, const Call& call, const std::string& shortcut = "", bool selected = false, bool enabled = true) {
        if (ImGui::MenuItem(label.c_str(), shortcut.c_str(), selected, enabled)) call();
    }

    inline void imTabBar(const std::string& label, const Call& call, ImGuiTabBarFlags flags = 0) {
        ImGui::BeginTabBar(label.c_str(), flags);
        call();
        ImGui::EndTabBar();
    }

    inline void imTabItem(const std::string& label, const Call& call, bool* open = nullptr, ImGuiTabItemFlags flags = 0) {
        if (ImGui::BeginTabItem(label.c_str(), open, flags)) {
            call();
            ImGui::EndTabItem();
        }
    }

    template <typename... T>
    inline void imText(fmt::format_string<T...> fmt, T&&... args) {
        ImGui::Text(vformat(fmt, fmt::make_format_args(args...)).c_str());
    }

    inline std::string imInputText(const std::string& label, const std::string& text = "", uint32_t size = 30) {
        std::vector<char> buffer(size);
        std::copy(text.begin(), text.end(), buffer.begin());
        ImGui::InputText(label.c_str(), buffer.data(), size);
        return {buffer.data(), size};
    }

    inline void imCollapsingHeader(const std::string& label, const Call& call, ImGuiTreeNodeFlags flags = 0) {
        if (ImGui::CollapsingHeader(label.c_str(), flags)) {
            call();
        }
    }

    inline void imInputVec3(const std::string& label, Vector3& vector, const std::string& format = "%.3f", ImGuiInputTextFlags flags = 0) {
        ImGui::InputFloat3(label.c_str(), vector.values, format.c_str(), flags);
    }

    inline void imPopupContextWindow(const Call& call) {
        if (ImGui::BeginPopupContextWindow()) {
            call();
            ImGui::EndPopup();
        }
    }

    inline bool imTreeNodeEx(const std::string& label, ImGuiTreeNodeFlags flags = 0) {
        return ImGui::TreeNodeEx(label.c_str(), flags);
    }

    inline void imPopupContextItem(const Call& call) {
        if (ImGui::BeginPopupContextItem()) {
            call();
            ImGui::EndPopup();
        }
    }

    inline bool imIsClicked(ImGuiMouseButton mouseButton = 0) {
        return ImGui::IsItemClicked(mouseButton);
    }

    inline void imSelectable(const std::string& label, const Call& call, bool selected = false, ImGuiSelectableFlags flags = 0, const Vector2 &size = {0, 0}) {
        if (ImGui::Selectable(label.c_str(), selected, flags, {size.x, size.y})) call();
    }

} // namespace re::ui


#endif //RAVENENGINE_IMELEMENTS_HPP
