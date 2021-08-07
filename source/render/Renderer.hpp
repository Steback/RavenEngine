#ifndef RAVENENGINE_RENDERER_HPP
#define RAVENENGINE_RENDERER_HPP


#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"


namespace re {

    class Window;

    class Renderer {
    public:
        Renderer(const eastl::string& appName);

        ~Renderer();

        [[nodiscard]] bool isWindowOpen() const;

    private:
        eastl::shared_ptr<Window> window;
    };

} // namespace re


#endif //RAVENENGINE_RENDERER_HPP
