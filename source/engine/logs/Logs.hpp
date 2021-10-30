#ifndef RAVENENGINE_LOGS_HPP
#define RAVENENGINE_LOGS_HPP


#include "spdlog/spdlog.h"

#include "LogsManager.hpp"
#include "engine/utils/Utils.hpp"


namespace re::log {

    const char* const DEFAULT_FILE_NAME = "runtime.log";

    inline void addFile(const std::string& name) {
        LogsManager::addFile(name.c_str());
    }

    inline void error(const std::string& message, const std::string& file = DEFAULT_FILE_NAME) {
#ifdef RE_DEBUG
        spdlog::error(message);
#endif
        LogsManager::writeFile(file, spdlog::level::err, message);
    }

    inline void info(const std::string& message, const std::string& file = DEFAULT_FILE_NAME) {
#ifdef RE_DEBUG
        spdlog::info(message);
#endif
        LogsManager::writeFile(file, spdlog::level::info, message);
    }

    inline void warn(const std::string& message, const std::string& file = DEFAULT_FILE_NAME) {
#ifdef RE_DEBUG
        spdlog::warn(message);
#endif
        LogsManager::writeFile(file, spdlog::level::warn, message);
    }

    // TODO: Check debug logs
    inline void debug(const std::string& message, const std::string& file = DEFAULT_FILE_NAME) {
#ifdef RE_DEBUG
        spdlog::debug(message);
#endif
        LogsManager::writeFile(file, spdlog::level::debug, message);
    }

    inline void critical(const std::string& message, const std::string& file = DEFAULT_FILE_NAME) {
#ifdef RE_DEBUG
        spdlog::critical(message);
#endif
        LogsManager::writeFile(file, spdlog::level::critical, message);
    }

    inline void trace(const std::string& message, const std::string& file = DEFAULT_FILE_NAME) {
#ifdef RE_DEBUG
        spdlog::trace(message);
#endif
        LogsManager::writeFile(file, spdlog::level::trace, message);
    }

} // namespace re::logs


#endif //RAVENENGINE_LOGS_HPP
