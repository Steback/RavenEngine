#include "MeshRender.hpp"

#include "engine/assets/AssetsManager.hpp"
#include "engine/jobSystem/JobSystem.hpp"


namespace re {

    /**
     *
     * @param name Valid Model name
     * @param owner Valid pointer to Entity
     */
    MeshRender::MeshRender(const std::string& name, Entity* owner) : Component(owner) {
        setModel(name);
    }

    /**
     *
     * @param component JSON component data
     * @param owner Valid pointer to Entity
     */
    MeshRender::MeshRender(json &component, Entity *owner) : Component(owner) {
        serialize(component);
    }

    json MeshRender::serialize() {
        return {
            {"name", model->getName()}
        };
    }

    void MeshRender::serialize(json &component) {
        setModel(component["name"]);
    }

    void MeshRender::setModel(const std::string& name) {
        jobs::submit([=, this](){
            model = AssetsManager::getInstance()->add<Model>(name, name);
            enable = true;
        });
    }

} // namespace re
