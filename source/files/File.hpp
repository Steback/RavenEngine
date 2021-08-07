#ifndef RAVENENGINE_FILE_HPP
#define RAVENENGINE_FILE_HPP


#include <filesystem>
#include <string>
#include <vector>


namespace re {

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
         *
         * @return String with file name
         */
        [[nodiscard]] std::string getName() const;

    private:
        std::filesystem::path path;
    };

} // namespace re


#endif //RAVENENGINE_FILE_HPP
