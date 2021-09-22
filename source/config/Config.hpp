#ifndef RAVENENGINE_CONFIG_HPP
#define RAVENENGINE_CONFIG_HPP


#include <string>

#include "files/File.hpp"


namespace re {

    // TODO: Change all Config class and add Doxygen comments
    class Config {
    public:
        Config() = default;

        explicit Config(const std::string& name);

        ~Config() = default;

        void load();

        void save();

        [[nodiscard]] int getWidth() const;

        void setWidth(int width_);

        [[nodiscard]] int getHeight() const;

        void setHeight(int height_);

    private:
        File file;
        int width{};
        int height{};
    };

} // namespace re


#endif //RAVENENGINE_CONFIG_HPP
