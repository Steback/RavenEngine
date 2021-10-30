#include "LogsManager.hpp"

#include "spdlog/spdlog.h"

#include "engine/files/FilesManager.hpp"


namespace re::log {

    const char* const DEFAULT_LOGS_PATTERN = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v";

    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> LogsManager::files;

    void LogsManager::addFile(const char *name) {
        if (files.find(std::string(name)) == files.end()) {
            auto logger = spdlog::basic_logger_mt(name, (files::getPath("logs") / name).string());
            logger->set_pattern(DEFAULT_LOGS_PATTERN);
            files[name] = logger;
        }
    }

    void LogsManager::writeFile(const std::string& name, spdlog::level::level_enum lvl, const std::string &message) {
        switch (lvl) {
            case spdlog::level::trace:
                files[name]->trace(message);
                break;
            case spdlog::level::debug:
                files[name]->debug(message);
                break;
            case spdlog::level::info:
                files[name]->info(message);
                break;
            case spdlog::level::warn:
                files[name]->warn(message);
                break;
            case spdlog::level::err:
                files[name]->error(message);
                break;
            case spdlog::level::critical:
                files[name]->critical(message);
                break;
        }
    }

    void LogsManager::cleanLogsFiles() {
        for (auto& file : std::filesystem::directory_iterator(files::getPath("logs")))
            std::filesystem::remove(file);
    }

} // namespace re::logs