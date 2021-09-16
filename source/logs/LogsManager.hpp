#ifndef RAVENENGINE_LOGSMANAGER_HPP
#define RAVENENGINE_LOGSMANAGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace re {

    class Base;

    namespace logs {
        /**
         * @brief Static class with all log functionalities, using spdlog.
         */
        class LogsManager {
            friend re::Base;

            LogsManager();

        public:
            ~LogsManager();

            static LogsManager* getInstance();

            /**
             * @brief Creates a new file in the logs folder
             * @param name File name
             */
            void addFile(const char* name);

            /**
             *
             * @param name File name
             * @return shared pointer with the spdlog log file
             */
            std::shared_ptr<spdlog::logger> getFile(const char* name);

        private:
            static LogsManager* singleton;
            std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
        };

    } // namespace logs

} // namespace re


#endif //RAVENENGINE_LOGSMANAGER_HPP
