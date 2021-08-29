#include "FilesManager.hpp"

#include "File.hpp"
#include "utils/Macros.hpp"


namespace re {

    std::unordered_map<std::string, std::filesystem::path> FilesManager::paths;

    FilesManager::FilesManager() = default;

    void FilesManager::setupDefaultPaths() {
        std::filesystem::path root = std::filesystem::current_path();

        if (root.filename() == "samples" || root.filename() == "tests")
            root = root.parent_path();

#ifdef _WIN64
        if (root.filename() == "Release")
            root = root.parent_path().parent_path();
#else
        if (root.filename() == "bin")
            root = root.parent_path();
#endif

        if (!std::filesystem::exists(root / "bin"))
            RE_THROW_EX("Failed to setup root path");

        paths["root"] = root;

        addPath("logs", true);
        addPath("assets");
        addPath("shaders");
        addPath("data");
        addPath("data/scenes");
    }

    void FilesManager::addPath(const char* name, bool create) {
        paths[name] = paths["root"] / name;

        if (create) std::filesystem::create_directory(paths[name]);
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