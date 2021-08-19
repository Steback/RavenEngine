#ifndef RAVENENGINE_ASSETSMANAGER_HPP
#define RAVENENGINE_ASSETSMANAGER_HPP


#include <memory>


namespace re {

    class Device;

    /**
     * Assets Manager class
     */
    class AssetsManager {
    public:
        /**
         * Default Constructor
         * @param device shared_ptr of Device class
         */
        explicit AssetsManager(std::shared_ptr<Device> device);

        ~AssetsManager();

    private:
        std::shared_ptr<Device> device;
    };

} // namespace re


#endif //RAVENENGINE_ASSETSMANAGER_HPP
