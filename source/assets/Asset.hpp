#ifndef RAVENENGINE_ASSET_HPP
#define RAVENENGINE_ASSET_HPP


#include <string>

#include "utils/NonCopyable.hpp"


namespace re {

    class Asset : NonCopyable {
    public:
        explicit Asset(std::string name);

        [[nodiscard]] std::string getName() const;

    protected:
        std::string name;
    };

} // namespace re


#endif //RAVENENGINE_ASSET_HPP
