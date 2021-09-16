#include "spdlog/spdlog.h"

#include "DamagedHelmet.hpp"


int main(int argc, char** arg) {
    CLI::App app("Raven Engine CLI options");
    re::Cli::setupOptions(app);
    CLI11_PARSE(app, argc, arg);

    try {
        DamagedHelmet gltf;
        gltf.run();
    } catch (const std::exception& e) {
        RE_ERROR_LOG(e.what());
    }

    return 0;
};
