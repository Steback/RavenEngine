#include "File.hpp"

#include <fstream>

#include "utils/Macros.hpp"


namespace re {

    File::File() = default;

    File::File(std::filesystem::path path) : path(std::move(path)) {

    }

    std::vector<uint32_t> File::readBytes() {
        std::ifstream file{path, std::ios::ate | std::ios::binary};

        if (!file.is_open()) RE_THROW_EX("Failed to open file: " + path.string());

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> buffer(fileSize);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(fileSize));

        file.close();

        return buffer;
    }

    std::string File::getName() const {
        return path.filename().string();
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

} // namespace re