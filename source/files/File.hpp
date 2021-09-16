#ifndef RAVENENGINE_FILE_HPP
#define RAVENENGINE_FILE_HPP


#include <filesystem>
#include <string>
#include <vector>

#include "external/Json.hpp"


namespace re::files {

    /**
     * @brief Class with file management functionalities. read/write
     */
    class File {
    public:
        /**
         * @brief Default constructor
         */
        File();

        /**
         *
         * @param path File path
         */
        explicit File(std::filesystem::path path);

        /**
         * @brief Read the file in binary mode
         * @return vector of uint32 with all file data
         */
        std::vector<uint32_t> readBytes();

        /**
         * @brief Read the file without binary mode
         * @return Vector of char whit file content
         */
        [[nodiscard]] std::vector<char> read() const;

        /**
         * Read the file for json serialization
         * @param data nlohmann::json object
         */
        void read(json& data);

        /**
         * Write the file for json serialization
         * @param data nlohmann::json object
         */
        void write(json& data);

        /**
         * Write data to binary file
         * @param binary
         */
        void write(const std::vector<uint32_t>& binary);

        /**
         *
         * @return File name without extension
         */
        [[nodiscard]] std::string getName(bool extension = false) const;

        /**
         *
         * @return Absolute path of file
         */
        [[nodiscard]] std::string getPath() const;

        [[nodiscard]] std::string getExtension() const;

        void setPath(std::filesystem::path path_);

    private:
        std::filesystem::path path;
    };

} // namespace re::files

using File = re::files::File;


#endif //RAVENENGINE_FILE_HPP
