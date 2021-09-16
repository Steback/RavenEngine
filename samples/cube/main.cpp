#include "spdlog/spdlog.h"

#include "Cube.hpp"


int main(int argc, char** arg) {
    CLI::App app("Raven Engine CLI flags");

    try {
        Cube cube(app);
        CLI11_PARSE(app, argc, arg);
        cube.run();
    } catch (const std::exception& e) {
        re::logs::error(e.what());
    }

    return 0;
};
