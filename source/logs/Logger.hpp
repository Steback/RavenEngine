#ifndef RAVENENGINE_LOGGER_HPP
#define RAVENENGINE_LOGGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    class Logger {
    public:
        static void setup();

        static void addFile(const char* name);

        static std::shared_ptr<spdlog::logger> getFile(const char* name);

    private:
        Logger() = default;

    private:
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
    };

} // namespace re


#endif //RAVENENGINE_LOGGER_HPP
