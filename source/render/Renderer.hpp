#ifndef RAVENENGINE_RENDERER_HPP
#define RAVENENGINE_RENDERER_HPP


#include <string>
#include <memory>


namespace re {

    class Window;

    /**
     * @brief Main render class, execute all render commands with Vulkan API
     */
    class Renderer {
    public:
        /**
         *
         * @param appName Application/Game name
         */
        Renderer(const std::string& appName);

        ~Renderer();

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
