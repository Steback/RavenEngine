#ifndef RAVENENGINE_CUBE_HPP
#define RAVENENGINE_CUBE_HPP


#include "Base.hpp"


class Cube : public re::Base {
public:
    Cube();

    ~Cube() override;

    void onUpdate() override;

private:

};


#endif //RAVENENGINE_CUBE_HPP
