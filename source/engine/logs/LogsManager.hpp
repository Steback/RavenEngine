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

        public:
            static void addFile(const char* name);

            static void writeFile(const std::string& name, spdlog::level::level_enum lvl, const std::string &message);

            static void cleanLogsFiles();

        private:
            static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
        };

    } // namespace logs

} // namespace re


#endif //RAVENENGINE_LOGSMANAGER_HPP
