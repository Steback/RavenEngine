#ifndef RAVENENGINE_FILESMANAGER_HPP
#define RAVENENGINE_FILESMANAGER_HPP


#include <filesystem>
#include <unordered_map>
#include <string>
#include <functional>

#include "File.hpp"
#include "engine/core/NonCopyable.hpp"


namespace re {

    class Application;

    namespace files {

        class FilesManager : NonCopyable {
            friend re::Application;

        public:
            static void setRootPath();

            static void addPath(const char* name, bool create = false);

            static std::filesystem::path getPath(const char* name);

            static File getFile(const char* name);

        private:
            static std::unordered_map<std::string, std::filesystem::path> paths;
        };

        inline File getFile(const std::string& name) {
            return FilesManager::getFile(name.c_str());
        }

        inline void addPath(const std::string& name, bool create = false) {
            FilesManager::addPath(name.c_str(), create);
        }

        inline std::filesystem::path getPath(const std::string& name) {
            return FilesManager::getPath(name.c_str());
        }

    } // namespace files

} // namespace re


using FilesManager = re::files::FilesManager;


#endif //RAVENENGINE_FILESMANAGER_HPP
