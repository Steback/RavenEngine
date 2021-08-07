#ifndef RAVENENGINE_LOGGER_HPP
#define RAVENENGINE_LOGGER_HPP


#include "EASTL/string.h"
#include "EASTL/hash_map.h"
#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    class Logger {
    public:
        static void setup();

        static void addFile(const eastl::string& name);

        static std::shared_ptr<spdlog::logger> getFile(const eastl::string& name);

    private:
        Logger() = default;

    private:
        static eastl::hash_map<eastl::string, std::shared_ptr<spdlog::logger>> files;
    };

} // namespace re


#endif //RAVENENGINE_LOGGER_HPP
