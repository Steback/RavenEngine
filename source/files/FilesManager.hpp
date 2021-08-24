#ifndef RAVENENGINE_FILESMANAGER_HPP
#define RAVENENGINE_FILESMANAGER_HPP


#include <filesystem>
#include <unordered_map>
#include <string>


namespace re {

    class File;

    /**
     * @brief Static class for all files and paths management.
     *
     * Not need a instance.
     */
    class FilesManager {
    public:
        /**
         * @bried Setup all default paths needed by the engine.
         * Default paths:
         * root(Project folder),
         * assets,
         * logs,
         * data,
         * shaders
         */
        static void setupDefaultPaths();

        /**
         * @brief Add a new path for the search paths
         * @param name New path name
         * @param create Create directory of path. By default is false
         */
        static void addPath(const char* name, bool create = false);

        /**
         *
         * @param name Path name
         * @return std::path object will the specific path if exists
         */
        static std::filesystem::path getPath(const char* name);

        /**
         *
         * @param name File name
         * @return File object with the specific file if exists
         */
        static File getFile(const char* name);

    private:
        FilesManager();

    private:
        static std::unordered_map<std::string, std::filesystem::path> paths;
    };

} // namespace re


#endif //RAVENENGINE_FILESMANAGER_HPP
