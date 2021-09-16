#include "File.hpp"

#include <fstream>
#include <utility>

#include "utils/Macros.hpp"


namespace re::files {

    File::File() = default;

    File::File(std::filesystem::path path) : path(std::move(path)) {

    }

    std::vector<uint32_t> File::readBytes() {
        std::ifstream file{path, std::ios::ate | std::ios::binary};

        // TODO: Change this throw to another position
        if (!file.is_open()) RE_THROW_EX("Failed to open file: " + path.string());

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> buffer(fileSize);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(fileSize));

        file.close();

        return buffer;
    }

    std::vector<char> File::read() const {
        std::ifstream file{path, std::ios::ate};

        if (!file.is_open()) RE_THROW_EX("Failed to open file: " + path.string());

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

        file.close();

        return buffer;
    }

    void File::read(json &data) {
        std::ifstream file(path);

        if (!file.is_open()) RE_THROW_EX("Failed to open file: " + path.string());

        file >> data;
        file.close();
    }

    void File::write(json &data) {
        std::ofstream file(path);

        if (!file.is_open()) RE_THROW_EX("Failed to open file: " + path.string());

        file << std::setw(4) << data;
        file.close();
    }

    void File::write(const std::vector<uint32_t> &binary) {
        std::ofstream file(path, std::ios::binary);

        if (!file.is_open()) RE_THROW_EX("Failed to open file: " + path.string());

        file.write(reinterpret_cast<const char *>(binary.data()), sizeof(uint32_t) * binary.size());

        file.close();
    }

    std::string File::getName(bool extension) const {
        std::string fileName = path.filename().string();

        if (extension) {
            size_t idx = fileName.rfind('.');
            fileName = fileName.substr(0, idx);
        }

        return fileName;
    }

    std::string File::getPath() const {
        return path.string();
    }

    std::string File::getExtension() const {
        return path.extension().string();
    }

    void File::setPath(std::filesystem::path path_) {
        path = std::move(path_);
    }

} // namespace re