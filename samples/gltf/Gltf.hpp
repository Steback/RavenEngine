#ifndef RAVENENGINE_GLTF_HPP
#define RAVENENGINE_GLTF_HPP


#include "Base.hpp"


class Gltf : public re::Base {
public:
    Gltf();

    ~Gltf() override;

    void onUpdate() override;

    void onDrawImGui() override;

private:

};


#endif //RAVENENGINE_GLTF_HPP
