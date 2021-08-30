#include "Cli.hpp"


namespace re {

    std::unordered_map<std::string, bool> Cli::options;

    void Cli::setupOptions(CLI::App &app) {
        addOption("compile-shaders", app);
    }

    void Cli::addOption(const std::string &name, CLI::App &app) {
        options[name] = false;
        app.add_flag("--" + name, options[name], "Compile shaders and save then in \"shaders\" director before load all core functionalities");
    }

    bool Cli::getOption(const std::string &name) {
        return options[name];
    }

} // namespace re
