#ifndef RAVENENGINE_FILE_HPP
#define RAVENENGINE_FILE_HPP


#include <filesystem>

#include "EASTL/string.h"
#include "EASTL/vector.h"


namespace re {

    class File {
    public:
        File();

        explicit File(std::filesystem::path  path);

        eastl::vector<uint32_t> readBytes();

        [[nodiscard]] eastl::string getName() const;

    private:
        std::filesystem::path path;
    };

} // namespace re


#endif //RAVENENGINE_FILE_HPP
