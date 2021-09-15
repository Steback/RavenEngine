#ifndef RAVENENGINE_CLI_HPP
#define RAVENENGINE_CLI_HPP


#include <string>
#include <unordered_map>

#include "CLI/App.hpp"


namespace re {

    class Cli {
        Cli() = default;

    public:
        /**
         * @brief Setup default cli options for engine
         * @param app CLI11 App
         *
         * Options:\n
         * compile-shaders
         */
        static void setupOptions(CLI::App& app);

        /**
         * @brief Add option to options map and cli app
         * @param name
         * @param app
         */
        static void addOption(const std::string& name, CLI::App& app);

        /**
         *
         * @param name
         * @return True if option was added
         */
        static bool getOption(const std::string& name);

    private:
        static std::unordered_map<std::string, bool> options;
    };

} // namespace re


#endif //RAVENENGINE_CLI_HPP
