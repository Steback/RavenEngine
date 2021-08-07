#include "Logger.hpp"

#include "spdlog/spdlog.h"

#include "files/FilesManager.hpp"


namespace re {

    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::files;

    void Logger::setup() {
        addFile("error.log");
    }

    void Logger::addFile(const char* name) {
        files[name] = spdlog::basic_logger_mt("RE", FilesManager::getPath("logs") / name);
    }

    std::shared_ptr<spdlog::logger> Logger::getFile(const char* name) {
        return files[name];
    }

} // namespace re