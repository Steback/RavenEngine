#include "spdlog/spdlog.h"

#include "Cube.hpp"


int main() {
    re::FilesManager::setupDefaultPaths(std::filesystem::current_path().parent_path());
    re::Logger::setup();

    try {
        Cube cube;
        cube.run();
    } catch (const std::exception& e) {
        RE_ERROR_LOG(e.what());
    }

    return 0;
};
