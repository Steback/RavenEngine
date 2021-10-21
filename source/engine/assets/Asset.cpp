#include "Asset.hpp"


namespace re {

    Asset::Asset(std::string name, Type type) : name(std::move(name)), type(type) {

    }

    std::string Asset::getName() const {
        return name;
    }

} // namespace re

