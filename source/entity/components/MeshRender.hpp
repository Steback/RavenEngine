#ifndef RAVENENGINE_MESHRENDER_HPP
#define RAVENENGINE_MESHRENDER_HPP


#include <memory>

#include "Component.hpp"
#include "assets/Model.hpp"


namespace re {

    class MeshRender : public Component {
    public:
        MeshRender(const std::string& name, Entity* owner);

        MeshRender(json& component, Entity* owner);

        json serialize() override;

        void serialize(json &component) override;

        void setModel(const std::string& name);

    public:
        Model* model{};
        bool enable{};
    };

} // namespace re


#endif //RAVENENGINE_MESHRENDER_HPP
