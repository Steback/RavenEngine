#include "LogsManager.hpp"

#include "spdlog/spdlog.h"

#include "files/FilesManager.hpp"


namespace re::logs {

    LogsManager* LogsManager::singleton;

    LogsManager::LogsManager() {
        files::addPath("logs", true);

        for (auto& file : std::filesystem::directory_iterator(files::getPath("logs"))) {
            std::filesystem::remove(file);
        }

        addFile("error.log");
    }

    LogsManager::~LogsManager() = default;

    LogsManager *LogsManager::getInstance() {
        return singleton;
    }

    void LogsManager::addFile(const char* name) {
        files[name] = spdlog::basic_logger_mt("RE", (files::getPath("logs") / name).string());
    }

    std::shared_ptr<spdlog::logger> LogsManager::getFile(const char* name) {
        return files[name];
    }

} // namespace re::logs