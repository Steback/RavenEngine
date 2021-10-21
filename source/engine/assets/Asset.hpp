#ifndef RAVENENGINE_ASSET_HPP
#define RAVENENGINE_ASSET_HPP


#include <string>

#include "engine/utils/NonCopyable.hpp"


namespace re {

    class Asset : NonCopyable {
    public:
        enum Type {
            MESH = 1,
            MODEL = 2,
            TEXTURE = 3,
            MATERIAL = 4
        };

        explicit Asset(std::string name, Type type);

        [[nodiscard]] std::string getName() const;

        const Type type;

    protected:
        std::string name;
    };

} // namespace re


#endif //RAVENENGINE_ASSET_HPP
