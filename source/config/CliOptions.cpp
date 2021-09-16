#include "CliOptions.hpp"


namespace re::cli {

    CliOptions* CliOptions::singleton;

    CliOptions::CliOptions(CLI::App& app) : app(app) {

    }

    CliOptions *CliOptions::getInstance() {
        return singleton;
    }

    void CliOptions::addFlag(const std::string &name, const std::string& desc, const std::string& flagName) {
        flags[name] = false;
        app.add_flag(flagName.empty() ? "--" + name : flagName, flags[name], desc);
    }

    bool CliOptions::getFlag(const std::string &name) {
        return flags[name];
    }

} // namespace re
