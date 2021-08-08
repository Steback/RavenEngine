#ifndef RAVENENGINE_RENDERER_HPP
#define RAVENENGINE_RENDERER_HPP


#include <string>
#include <memory>

#include "utils/NonCopyable.hpp"


namespace re {

    class Window;
    class Config;

    /**
     * @brief Main render class, execute all render commands with Vulkan API
     */
    class Renderer : NonCopyable {
    public:
        /**
         *
         * @param appName Application/Game name
         */
        Renderer(const std::string& appName, const Config& config);

        ~Renderer() override;

        /**
         *
         * @return bool value that depend if main windows is currently open or not
         */
        [[nodiscard]] bool isWindowOpen() const;

    private:
        std::shared_ptr<Window> window;
    };

} // namespace re


#endif //RAVENENGINE_RENDERER_HPP
