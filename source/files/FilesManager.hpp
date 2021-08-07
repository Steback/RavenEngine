#ifndef RAVENENGINE_FILESMANAGER_HPP
#define RAVENENGINE_FILESMANAGER_HPP


#include <filesystem>

#include "EASTL/string.h"
#include "EASTL/hash_map.h"


namespace re {

    class File;

    class FilesManager {
    public:
        static void setupDefaultPaths();

        static void addPath(const char* name);

        static std::filesystem::path getPath(const char* name);

        static File getFile(const char* name);

    private:
        FilesManager();

    private:
        static eastl::hash_map<eastl::string, std::filesystem::path> paths;
    };

} // namespace re


#endif //RAVENENGINE_FILESMANAGER_HPP
