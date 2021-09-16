#include "spdlog/spdlog.h"

#include "Cube.hpp"


int main(int argc, char** arg) {
    CLI::App app("Raven Engine CLI options");
    re::Cli::setupOptions(app);
    CLI11_PARSE(app, argc, arg);

    try {
        Cube cube;
        cube.run();
    } catch (const std::exception& e) {
        RE_ERROR_LOG(e.what());
    }

    return 0;
};
