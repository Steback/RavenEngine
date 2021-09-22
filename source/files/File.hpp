#ifndef RAVENENGINE_FILE_HPP
#define RAVENENGINE_FILE_HPP


#include <filesystem>
#include <string>
#include <vector>

#include "external/Json.hpp"


namespace re::files {

    // TODO: Should File class be refactored?
    class File {
    public:
        File();

        explicit File(std::filesystem::path path);

        std::vector<uint32_t> readBytes();

        [[nodiscard]] std::vector<char> read() const;

        void read(json& data);

        void write(json& data);

        void write(const std::vector<uint32_t>& binary);

        [[nodiscard]] std::string getName(bool extension = false) const;

        [[nodiscard]] std::string getPath() const;

        [[nodiscard]] std::string getExtension() const;

        void setPath(std::filesystem::path path_);

    private:
        std::filesystem::path path;
    };

} // namespace re::files

using File = re::files::File;


#endif //RAVENENGINE_FILE_HPP
