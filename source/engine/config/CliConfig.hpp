#ifndef RAVENENGINE_CLICONFIG_HPP
#define RAVENENGINE_CLICONFIG_HPP


#include <string>
#include <unordered_map>

#include "CLI/App.hpp"


namespace re {

    class Base;

    namespace cli {

        class CliConfig {
            friend re::Base;

            explicit CliConfig(const std::string& appName);

        public:
            static CliConfig* instance();

            void addFlag(const std::string &name, const std::string& desc, const std::string& flagName = "");

            bool getFlag(const std::string& name);

            void checkFlags();

            CLI::App& getApp();

        private:
            static CliConfig* singleton;
            CLI::App app;
            std::unordered_map<std::string, bool> flags;
        };

        inline void addFlag(const std::string &name, const std::string& desc, const std::string& flagName = "") {
            CliConfig::instance()->addFlag(name, desc, flagName);
        }

        inline bool getFlag(const std::string& name) {
            return CliConfig::instance()->getFlag(name);
        }

        inline void checkFlags() {
            CliConfig::instance()->checkFlags();
        }

        inline CLI::App& getApp() {
            return CliConfig::instance()->getApp();
        }

    } // namespace cli

} // namespace re


#endif //RAVENENGINE_CLICONFIG_HPP
