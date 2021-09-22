#ifndef RAVENENGINE_FILESMANAGER_HPP
#define RAVENENGINE_FILESMANAGER_HPP


#include <filesystem>
#include <unordered_map>
#include <string>
#include <functional>

#include "File.hpp"
#include "utils/NonCopyable.hpp"


namespace re {

    class Base;

    namespace files {

        class FilesManager : NonCopyable {
            friend re::Base;

            FilesManager();

        public:
            ~FilesManager() override;

            static FilesManager* getInstance();

            void addPath(const char* name, bool create = false);

            std::filesystem::path getPath(const char* name);

            File getFile(const char* name);

        private:
            static FilesManager* singleton;
            std::unordered_map<std::string, std::filesystem::path> paths;
        };

        inline File getFile(const std::string& name) {
            return FilesManager::getInstance()->getFile(name.c_str());
        }

        inline void addPath(const std::string& name, bool create = false) {
            FilesManager::getInstance()->addPath(name.c_str(), create);
        }

        inline std::filesystem::path getPath(const std::string& name) {
            return FilesManager::getInstance()->getPath(name.c_str());
        }

    } // namespace files

} // namespace re


using FilesManager = re::files::FilesManager;


#endif //RAVENENGINE_FILESMANAGER_HPP
