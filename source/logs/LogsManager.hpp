#ifndef RAVENENGINE_LOGSMANAGER_HPP
#define RAVENENGINE_LOGSMANAGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    class Base;

    namespace logs {
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
            std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
        };

    } // namespace logs

} // namespace re


#endif //RAVENENGINE_LOGSMANAGER_HPP
