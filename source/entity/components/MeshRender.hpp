#ifndef RAVENENGINE_MESHRENDER_HPP
#define RAVENENGINE_MESHRENDER_HPP


#include <memory>

#include "Component.hpp"
#include "assets/Model.hpp"


namespace re {

    /**
     * @brief Mesh render class
     * Have the Model pointer
     */
    class MeshRender : public Component {
    public:
        /**
         *
         * @param model Valid pointer to Model
         */
        explicit MeshRender(std::shared_ptr<Model> model);

        /**
         *
         * @param model Valid pointer to Model
         * @param owner Valid pointer to entity that owns this component
         */
        MeshRender(std::shared_ptr<Model> model, Entity* owner);

        /**
         * @brief Serialize MeshRender to JSON
         */
        json serialize() override;

        /**
         * @brief Serialize MeshRender from JSON
         *
         * This is not used so far
         */
        void serialize(json &component) override;

    public:
        std::shared_ptr<Model> model;
    };

} // namespace re


#endif //RAVENENGINE_MESHRENDER_HPP
