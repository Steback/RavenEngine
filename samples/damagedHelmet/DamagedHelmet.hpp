#ifndef RAVENENGINE_DAMAGEDHELMET_HPP
#define RAVENENGINE_DAMAGEDHELMET_HPP


#include "Base.hpp"


class DamagedHelmet : public re::Base {
public:
    DamagedHelmet(CLI::App& app);

    ~DamagedHelmet() override;

    void onUpdate() override;

    void onDrawImGui() override;

    void onLoadScene() override;

private:
    re::vec3 angles;
    std::shared_ptr<re::Entity> entity;
    std::shared_ptr<re::Entity> light;
};


#endif //RAVENENGINE_DAMAGEDHELMET_HPP
