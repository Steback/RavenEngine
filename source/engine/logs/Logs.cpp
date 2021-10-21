#include "Logs.hpp"

namespace re::logs {

    /**
     * @brief Send a log message to file.
     * @param message Log message
     * @param type [Optional] Severity type of message
     * @param file [Optional] File to send message. Need to be added before.
     */
    void log(const std::string& message, Type type, const std::string& file) {
#ifdef RE_DEBUG
        switch (type) {
            case INFO:
                spdlog::info(message);
                break;
            case ERROR:
                spdlog::error(message);
                break;
            case WARNING:
                spdlog::warn(message);
                break;
        }
#endif
        switch (type) {
            case INFO:
                LogsManager::getInstance()->getFile(file.c_str())->info(message);
                break;
            case ERROR:
                LogsManager::getInstance()->getFile(file.c_str())->error(message);
                break;
            case WARNING:
                LogsManager::getInstance()->getFile(file.c_str())->warn(message);
                break;
        }
    }

} // namespace re::logs
