#ifndef RAVENENGINE_LOGGER_HPP
#define RAVENENGINE_LOGGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    /**
     * @brief Static class with all log functionalities, using spdlog.
     *
     * Not need a instance.
     */
    class Logger {
    public:
        /**
         * @brief Creates all default logs files needed by the engine.
         *
         * For now just `error.log` file are created
         */
        static void setup();

        /**
         * @brief Creates a new file in the logs folder
         * @param name File name
         */
        static void addFile(const char* name);

        /**
         *
         * @param name File name
         * @return shared pointer with the spdlog log file
         */
        static std::shared_ptr<spdlog::logger> getFile(const char* name);

    private:
        Logger() = default;

    private:
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
    };

} // namespace re


#endif //RAVENENGINE_LOGGER_HPP
