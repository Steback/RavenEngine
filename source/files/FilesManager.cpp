#include "FilesManager.hpp"

#include "File.hpp"


namespace re {

    std::unordered_map<std::string, std::filesystem::path> FilesManager::paths;

    FilesManager::FilesManager() = default;

    void FilesManager::setupDefaultPaths() {
        std::filesystem::path root = std::filesystem::current_path();

        if (root.filename() == "bin")
            root = root.parent_path();

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
                return File(path);
        }

        return {};
    }

} // namespace re