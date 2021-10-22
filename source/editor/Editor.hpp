#ifndef RAVENENGINE_EDITOR_HPP
#define RAVENENGINE_EDITOR_HPP


#include "engine/Base.hpp"


namespace re {

    class Editor : public Base {
    public:
        explicit Editor(CLI::App& app);
    };

}

#endif //RAVENENGINE_EDITOR_HPP
