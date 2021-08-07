#include "spdlog/spdlog.h"

#include "Cube.hpp"
#include "logs/Logs.hpp"


int main() {
    re::Logger::setup();

    try {
        Cube cube;
        cube.run();
    } catch (const std::exception& e) {
        RE_ERROR_LOG(e.what());
    }

    return 0;
};
