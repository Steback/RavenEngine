#include "Sponza.hpp"


int main(int argc, char** arg) {
    CLI::App app("Raven Engine CLI flags");

    try {
        Sponza sample(app);
        CLI11_PARSE(app, argc, arg);
        sample.run();
    } catch (const std::exception& e) {
        re::logs::log(e.what());
    }

    return 0;
};
