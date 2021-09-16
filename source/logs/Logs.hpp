#ifndef RAVENENGINE_LOGS_HPP
#define RAVENENGINE_LOGS_HPP


#include "spdlog/spdlog.h"

#include "LogsManager.hpp"
#include "utils/Macros.hpp"


namespace re::logs {

    /**
     * @brief Send a message to default error file
     */
    inline void error(const std::string& message) {
#ifdef RE_DEBUG
        spdlog::error(message);
#endif
        LogsManager::getInstance()->getFile("error.log")->error(message);
    }

    inline void addFile(const std::string& name) {
        LogsManager::getInstance()->addFile(name.c_str());
    }

    inline std::shared_ptr<spdlog::logger> getFile(const std::string& name) {
        return LogsManager::getInstance()->getFile(name.c_str());
    }

} // namespace re::logs


#endif //RAVENENGINE_LOGS_HPP
