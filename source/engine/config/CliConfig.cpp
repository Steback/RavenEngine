#include "CliConfig.hpp"

#include "engine/core/Utils.hpp"
#include "engine/logs/Logs.hpp"


namespace re::cli {

    CliConfig* CliConfig::singleton;

    /**
     *
     * @param app CLI11 App
     */
    CliConfig::CliConfig(const std::string& appName) {
        CLI::App(appName + " CLI");
    }

    CliConfig *CliConfig::instance() {
        return singleton;
    }

    /**
     *
     * @param name Flag name. Remember add -- to name. If you want abbreviations like -x, add them(Example: "-f, --flag")
     * @param desc Flag description
     */
    void CliConfig::addFlag(const std::string &name, const std::string& desc) {
#ifdef RE_DEBUG
        log::info(fmt::format("CLI flag: {}. Added to flags list", name));
#endif
        app.add_flag(name, desc);
    }

    /**
     * @brief Check if flags was used to program. Need to be added before.
     * @param name Flag name
     */
    bool CliConfig::getFlag(const std::string &name) {
        return app.get_option(name)->as<bool>();
    }

    CLI::App &CliConfig::getApp() {
        return app;
    }

} // namespace re
