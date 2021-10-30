#include "Editor.hpp"


int main(int argc, char** arg) {
    try {
        re::Editor editor;
        CLI11_PARSE(re::cli::getApp(), argc, arg);
        editor.run();
    } catch (const std::exception& e) {
        re::log::error(e.what());
    }

    return 0;
};

