#ifndef RAVENENGINE_MESHRENDER_HPP
#define RAVENENGINE_MESHRENDER_HPP


#include <memory>

#include "Component.hpp"
#include "assets/Model.hpp"


namespace re {

    class MeshRender : public Component {
    public:
        MeshRender(Model* model, Entity* owner);

        json serialize() override;

        void serialize(json &component) override;

    public:
        Model* model;
    };

} // namespace re


#endif //RAVENENGINE_MESHRENDER_HPP
