#ifndef RAVENENGINE_CUBE_HPP
#define RAVENENGINE_CUBE_HPP


#include "Base.hpp"


class Cube : public re::Base {
public:
    Cube();

    ~Cube() override;

    void onUpdate() override;

    void onDrawImGui() override;

private:
    std::shared_ptr<re::Entity> cube;
};


#endif //RAVENENGINE_CUBE_HPP
