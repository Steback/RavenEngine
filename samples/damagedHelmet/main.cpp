#include "spdlog/spdlog.h"

#include "DamagedHelmet.hpp"


int main(int argc, char** arg) {
    CLI::App app("Raven Engine CLI flags");

    try {
        DamagedHelmet gltf(app);
        CLI11_PARSE(app, argc, arg);
        gltf.run();
    } catch (const std::exception& e) {
        re::logs::log(e.what());
    }

    return 0;
};
