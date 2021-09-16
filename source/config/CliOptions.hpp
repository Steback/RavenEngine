#ifndef RAVENENGINE_CLIOPTIONS_HPP
#define RAVENENGINE_CLIOPTIONS_HPP


#include <string>
#include <unordered_map>

#include "CLI/App.hpp"


namespace re {

    class Base;

    namespace cli {

        class CliOptions {
            friend re::Base;

            explicit CliOptions(CLI::App& app);

        public:
            static CliOptions* getInstance();

            /**
             * @brief Add flag to CLI
             */
            void addFlag(const std::string &name, const std::string& desc, const std::string& flagName = "");

            /**
             * @brief Check if flag was added to program
             */
            bool getFlag(const std::string& name);

        private:
            static CliOptions* singleton;
            CLI::App& app;
            std::unordered_map<std::string, bool> flags;
        };

        /**
         * @brief Add flag to CLI
         */
        inline void addFlag(const std::string &name, const std::string& desc, const std::string& flagName = "") {
            CliOptions::getInstance()->addFlag(name, desc, flagName);
        }

        /**
         * @brief Check if flag was added to program
         */
        inline bool getOption(const std::string& name) {
            return CliOptions::getInstance()->getFlag(name);
        }

    } // namespace cli

} // namespace re


#endif //RAVENENGINE_CLIOPTIONS_HPP
