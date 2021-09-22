#include "MeshRender.hpp"


namespace re {

    /**
     *
     * @param model Valid pointer to Model
     */
    MeshRender::MeshRender(std::shared_ptr<Model> model) : Component(nullptr), model(std::move(model)) {

    }

    /**
     *
     * @param model Valid pointer to Model
     * @param owner Valid pointer to Entity
     */
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
