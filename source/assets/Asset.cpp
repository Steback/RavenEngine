#include "Asset.hpp"


namespace re {

    Asset::Asset(std::string name) : name(std::move(name)) {

    }

    std::string Asset::getName() const {
        return name;
    }

} // namespace re

