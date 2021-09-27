#include "MeshRender.hpp"


namespace re {

    /**
     *
     * @param model Valid pointer to Model
     * @param owner Valid pointer to Entity
     */
    MeshRender::MeshRender(Model* model, Entity* owner) : Component(owner), model(model) {

    }

    json MeshRender::serialize() {
        return {
            {"name", model->getName()}
        };
    }

    void MeshRender::serialize(json &component) {

    }

} // namespace re
