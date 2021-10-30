#ifndef RAVENENGINE_LOGSMANAGER_HPP
#define RAVENENGINE_LOGSMANAGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    class Base;

    namespace log {

        class LogsManager {
            friend Base;

            LogsManager();

        public:
            static LogsManager* instance();

            void addFile(const char* name);

            void writeFile(const std::string& name, spdlog::level::level_enum lvl, const std::string &message);

        private:
            static LogsManager* singleton;
            std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
        };

    } // namespace logs

} // namespace re


#endif //RAVENENGINE_LOGSMANAGER_HPP
