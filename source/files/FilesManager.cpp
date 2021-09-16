#include "FilesManager.hpp"

#include "utils/Utils.hpp"


namespace re::files {

    FilesManager* FilesManager::singleton;

    FilesManager::FilesManager() {
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
            throwEx("Failed to setup root path");

        paths["root"] = root;
    }

    FilesManager::~FilesManager() = default;

    FilesManager *FilesManager::getInstance() {
        return singleton;
    }

    void FilesManager::addPath(const char* name, bool create) {
        paths[name] = paths["root"] / name;

        if (create) std::filesystem::create_directory(paths[name]);
    }

    std::filesystem::path FilesManager::getPath(const char* name) {
        return paths[name];
    }

    // TODO: Improve feedback when file is not found
    File FilesManager::getFile(const char* name) {
        for (auto& [id, path] : paths) {
            std::filesystem::path filePath(path / name);
            if (std::filesystem::exists(filePath))
                return File(filePath);
        }

        return File(name);
    }

} // namespace re::files