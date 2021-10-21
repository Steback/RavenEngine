#ifndef RAVENENGINE_LOGS_HPP
#define RAVENENGINE_LOGS_HPP


#include "spdlog/spdlog.h"

#include "LogsManager.hpp"


namespace re::logs {


    void log(const std::string& message, Type type = INFO, const std::string& file = DEFAULT_LOGS_FILE_NAME);

    inline void addFile(const std::string& name) {
        LogsManager::getInstance()->addFile(name.c_str());
    }

    inline std::shared_ptr<spdlog::logger> getFile(const std::string& name) {
        return LogsManager::getInstance()->getFile(name.c_str());
    }

} // namespace re::logs


#endif //RAVENENGINE_LOGS_HPP
