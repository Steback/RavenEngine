#include "Config.hpp"

#include "files/FilesManager.hpp"


namespace re {

    Config::Config(const std::string &name) {
        file = FilesManager::getFile(name.c_str());
    }

    void Config::load() {
        json data;
        file.read(data);

        width = data["width"].get<int>();
        height = data["height"].get<int>();
    }

    void Config::save() {
        json data;
        data["width"] = width;
        data["height"] = height;

        file.write(data);
    }

    int Config::getWidth() const {
        return width;
    }

    void Config::setWidth(int width_) {
        Config::width = width_;
    }

    int Config::getHeight() const {
        return height;
    }

    void Config::setHeight(int height_) {
        Config::height = height_;
    }

} // namespace re