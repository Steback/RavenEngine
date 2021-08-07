#ifndef RAVENENGINE_FILE_HPP
#define RAVENENGINE_FILE_HPP


#include <filesystem>
#include <string>
#include <vector>


namespace re {

    class File {
    public:
        File();

        explicit File(std::filesystem::path  path);

        std::vector<uint32_t> readBytes();

        [[nodiscard]] std::string getName() const;

    private:
        std::filesystem::path path;
    };

} // namespace re


#endif //RAVENENGINE_FILE_HPP
