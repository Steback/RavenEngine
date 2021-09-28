#ifndef RAVENENGINE_LOGSMANAGER_HPP
#define RAVENENGINE_LOGSMANAGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    class Base;

    namespace logs {

        const char* const DEFAULT_LOGS_FILE_NAME = "runtime.log";
        const char* const DEFAULT_LOGS_PATTERN = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v";

        enum Type {
            INFO = 0,
            ERROR = 1,
            WARNING = 2
        };

        class LogsManager {
            friend re::Base;

            LogsManager();

        public:
            ~LogsManager();

            static LogsManager* getInstance();

            void addFile(const char* name);

            std::shared_ptr<spdlog::logger> getFile(const char* name);

        private:
            static LogsManager* singleton;
            std::unordered_map<uint32_t , std::shared_ptr<spdlog::logger>> files;
        };

    } // namespace logs

} // namespace re


#endif //RAVENENGINE_LOGSMANAGER_HPP
