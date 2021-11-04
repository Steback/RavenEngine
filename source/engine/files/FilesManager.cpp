#include "FilesManager.hpp"

#include "engine/core/Utils.hpp"


namespace re::files {

    std::unordered_map<std::string, std::filesystem::path> FilesManager::paths;


    /**
     * @brief Set project root path
     */
    void FilesManager::setRootPath() {
        std::filesystem::path root = std::filesystem::current_path();

        if (root.filename() == "samples" || root.filename() == "tests")
            root = root.parent_path();

#ifdef _WIN64
        if (root.filename() == "Release" || root.filename() == "Debug")
            root = root.parent_path().parent_path();
#else
        if (root.filename() == "bin")
            root = root.parent_path();
#endif

        if (!std::filesystem::exists(root / "bin"))
            throwEx("Failed to setup root path");

        paths["root"] = root;
    }

    /**
     * @brief Add a search path
     * @param name Path name
     * @param create [Optional] Create a directory in new path
     */
    void FilesManager::addPath(const char* name, bool create) {
        paths[name] = paths["root"] / name;

        if (create) std::filesystem::create_directory(paths[name]);
    }

    /**
     *
     * @param name Valid path name
     */
    std::filesystem::path FilesManager::getPath(const char* name) {
        return paths[name];
    }

    /**
     * @brief Find file in all search path. If not exists throw exception
     * @param name Valid file name
     */
    File FilesManager::getFile(const char* name) {
        for (auto& [id, path] : paths) {
            std::filesystem::path filePath(path / name);
            if (std::filesystem::exists(filePath))
                return File(filePath);
        }

        throwEx(fmt::format("Failed to find file: {}", name));

        return File(name);
    }

} // namespace re::files