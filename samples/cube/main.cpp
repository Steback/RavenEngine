#include "spdlog/spdlog.h"

#include "Cube.hpp"


int main() {
    try {
        Cube cube;
        cube.run();
    } catch (const std::exception& e) {
        spdlog::error(e.what());
    }

    return 0;
};
