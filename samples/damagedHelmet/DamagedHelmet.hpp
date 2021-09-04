#ifndef RAVENENGINE_DAMAGEDHELMET_HPP
#define RAVENENGINE_DAMAGEDHELMET_HPP


#include "Base.hpp"


class DamagedHelmet : public re::Base {
public:
    DamagedHelmet();

    ~DamagedHelmet() override;

    void onUpdate() override;

    void onDrawImGui() override;

private:
    re::vec3 angles;
    std::shared_ptr<re::Entity> entity;
};


#endif //RAVENENGINE_DAMAGEDHELMET_HPP
