#include "LogsManager.hpp"

#include "spdlog/spdlog.h"

#include "files/FilesManager.hpp"


namespace re::logs {

    LogsManager* LogsManager::singleton;

    /**
     * @brief Construct instance and clean up logs files
     */
    LogsManager::LogsManager() {
        spdlog::set_pattern(DEFAULT_LOGS_PATTERN);

        for (auto& file : std::filesystem::directory_iterator(files::getPath("logs"))) {
            std::filesystem::remove(file);
        }
    }

    LogsManager::~LogsManager() = default;

    /**
     *
     * @return Singleton instance
     */
    LogsManager *LogsManager::getInstance() {
        return singleton;
    }

    /**
     *
     * @param name File name to create
     */
    void LogsManager::addFile(const char* name) {
        uint32_t hashName = std::hash<std::string>()(name);

        if (files.find(hashName) == files.end()) {
            auto logger = spdlog::basic_logger_mt(name, (files::getPath("logs") / name).string());
            logger->set_pattern(DEFAULT_LOGS_PATTERN);
            files[hashName] = logger;
        }
    }

    /**
     *
     * @param name Valid file name
     * @return Pointer to spdlog::logger
     */
    std::shared_ptr<spdlog::logger> LogsManager::getFile(const char* name) {
        uint32_t hashName = std::hash<std::string>()(name);

        if (files.find(hashName) != files.end())
            return files[hashName];

        return nullptr;
    }

} // namespace re::logs