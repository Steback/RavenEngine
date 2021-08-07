#include "Logger.hpp"

#include "spdlog/spdlog.h"


namespace re {

    eastl::hash_map<eastl::string, std::shared_ptr<spdlog::logger>> Logger::files;

    void Logger::setup() {
        addFile("error.log");
    }

    void Logger::addFile(const eastl::string &name) {
        files[name] = spdlog::basic_logger_mt("RE", name.c_str());
    }

    std::shared_ptr<spdlog::logger> Logger::getFile(const eastl::string &name) {
        return files[name];
    }

} // namespace re