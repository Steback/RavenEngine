#include "AssetsManager.hpp"


namespace re {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {

    }

    AssetsManager::~AssetsManager() {

    }

} // namespace re