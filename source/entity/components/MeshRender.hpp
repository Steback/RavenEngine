#ifndef RAVENENGINE_MESHRENDER_HPP
#define RAVENENGINE_MESHRENDER_HPP


#include <memory>

#include "Component.hpp"
#include "assets/Model.hpp"


namespace re {

    class MeshRender : public Component {
    public:
        explicit MeshRender(std::shared_ptr<Model> model);

        MeshRender(std::shared_ptr<Model> model, Entity* owner);

        json serialize() override;

        void serialize(json &component) override;

    public:
        std::shared_ptr<Model> model;
    };

} // namespace re


#endif //RAVENENGINE_MESHRENDER_HPP
