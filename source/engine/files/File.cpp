#include "File.hpp"

#include <fstream>
#include <utility>

#include "engine/core/Utils.hpp"


namespace re::files {

    File::File() = default;

    /**
     *
     * @param path Valid file path
     */
    File::File(std::filesystem::path path) : path(std::move(path)) {

    }

    /**
     * @brief Read file with binary mode
     */
    std::vector<uint32_t> File::readBytes() {
        std::ifstream file{path, std::ios::ate | std::ios::binary};

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> buffer(fileSize);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(fileSize));

        file.close();

        return buffer;
    }

    /**
     * @brief Read file without binary mode
     */
    std::vector<char> File::read() const {
        std::ifstream file{path, std::ios::ate};

        if (!file.is_open()) throwEx("Failed to open file: " + path.string());

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

        file.close();

        return buffer;
    }

    /**
     * @brief Read file and save its data in JSON format(The file will be .json)
     * @param data Reference to JSON object
     */
    void File::read(json &data) {
        std::ifstream file(path);

        if (!file.is_open()) throwEx("Failed to open file: " + path.string());

        file >> data;
        file.close();
    }

    /**
     * @brief Write file in JSON format
     * @param data Reference to JSON object
     */
    void File::write(json &data) {
        std::ofstream file(path);

        if (!file.is_open()) throwEx("Failed to open file: " + path.string());

        file << std::setw(4) << data;
        file.close();
    }

    /**
     * @brief Write file in Binary format
     * @param binary Binary data
     */
    void File::write(const std::vector<uint32_t> &binary) {
        std::ofstream file(path, std::ios::binary);

        if (!file.is_open()) throwEx("Failed to open file: " + path.string());

        file.write(reinterpret_cast<const char *>(binary.data()), sizeof(uint32_t) * binary.size());

        file.close();
    }

    /**
     *
     * @param extension [Optional] Get the name without extension or not.
     * @return File name
     */
    std::string File::getName(bool extension) const {
        std::string fileName = path.filename().string();

        if (extension) {
            size_t idx = fileName.rfind('.');
            fileName = fileName.substr(0, idx);
        }

        return fileName;
    }

    /**
     *
     * @return File Path
     */
    std::string File::getPath() const {
        return path.string();
    }

    /**
     *
     * @return File extension
     */
    std::string File::getExtension() const {
        return path.extension().string();
    }

    /**
     *
     * @param path_ New path to set to file
     */
    void File::setPath(std::filesystem::path path_) {
        path = std::move(path_);
    }

} // namespace re