#include "FilesManager.hpp"

#include "File.hpp"


namespace re {

    std::unordered_map<std::string, std::filesystem::path> FilesManager::paths;

    FilesManager::FilesManager() = default;

    void FilesManager::setupDefaultPaths(const std::filesystem::path& rootPath) {
        std::filesystem::path root = rootPath;

#ifdef _WIN64
        if (root.filename() == "Release")
            root = root.parent_path().parent_path();
#else
        if (root.filename() == "bin")
            root = root.parent_path();
#endif

        paths["root"] = root;

        addPath("logs");
        addPath("assets");
        addPath("shaders");
        addPath("data");
        addPath("bin/shaders");
    }

    void FilesManager::addPath(const char* name) {
        paths[name] = paths["root"] / name;
    }

    std::filesystem::path FilesManager::getPath(const char* name) {
        return paths[name];
    }

    File FilesManager::getFile(const char* name) {
        for (auto& [id, path] : paths) {
            std::filesystem::path filePath(path / name);
            if (std::filesystem::exists(filePath))
                return File(filePath);
        }

        return {};
    }

} // namespace re