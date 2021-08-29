#include "MeshRender.hpp"


namespace re {

    MeshRender::MeshRender(std::shared_ptr<Model> model) : Component(nullptr), model(std::move(model)) {

    }

    MeshRender::MeshRender(std::shared_ptr<Model> model, Entity *owner) : Component(owner), model(std::move(model)) {

    }

    json MeshRender::serialize() {
        return {
            {"name", model->getName()}
        };
    }

    void MeshRender::serialize(json &component) {

    }

} // namespace re
