#ifndef RAVENENGINE_SPONZA_HPP
#define RAVENENGINE_SPONZA_HPP


#include "Base.hpp"


class Sponza : public re::Base {
public:
    explicit Sponza(CLI::App& app);

    ~Sponza() override;

    void onUpdate() override;

    void onDrawImGui() override;

    void onLoadScene() override;

private:
    std::shared_ptr<re::Entity> entity;
    re::vec3 eulerAngles;
};


#endif //RAVENENGINE_SPONZA_HPP
