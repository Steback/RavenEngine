#include "CliOptions.hpp"


namespace re::cli {

    CliOptions* CliOptions::singleton;

    /**
     *
     * @param app CLI11 App
     */
    CliOptions::CliOptions(CLI::App& app) : app(app) {

    }

    CliOptions *CliOptions::getInstance() {
        return singleton;
    }

    /**
     *
     * @param name Flag name(without --)
     * @param desc Flag description
     * @param flagName [Optional] Specific flag name(such as "--flag -f") with shortcut. Is necessarily add '--'
     */
    void CliOptions::addFlag(const std::string &name, const std::string& desc, const std::string& flagName) {
        flags[name] = false;
        app.add_flag(flagName.empty() ? "--" + name : flagName, flags[name], desc);
    }

    /**
     * @brief Check if flags was used to program. Need to be added before.
     * @param name Flag name
     */
    bool CliOptions::getFlag(const std::string &name) {
        return flags[name];
    }

} // namespace re
