#include "Editor.hpp"


int main(int argc, char** arg) {
    CLI::App app("Raven Engine CLI flags");

    try {
        re::Editor editor(app);
        CLI11_PARSE(app, argc, arg);
        editor.run();
    } catch (const std::exception& e) {
        re::log::error(e.what());
    }

    return 0;
};

