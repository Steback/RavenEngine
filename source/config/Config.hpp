#ifndef RAVENENGINE_CONFIG_HPP
#define RAVENENGINE_CONFIG_HPP


#include <string>

#include "files/File.hpp"


namespace re {

    /**
     * @brief Class for save all the default setting
     */
    class Config {
    public:
        Config() = default;

        /**
         *
         * @param name File name
         */
        explicit Config(const std::string& name);

        ~Config() = default;

        /**
         * @brief Load file in the path if exists
         */
        void load();

        /**
         * @brief Save file in the path
         */
        void save();

        /**
         * @brief Get the window width value in config
         * @return int value
         */
        [[nodiscard]] int getWidth() const;

        /**
         * @brief Set the window width value in config
         * @param width_ int value
         */
        void setWidth(int width_);

        /**
         * @brief Get the window height value in config
         * @return int value
         */
        [[nodiscard]] int getHeight() const;

        /**
         * @brief Set height window value in config
         * @param height_ int value
         */
        void setHeight(int height_);

    private:
        File file;
        int width{};
        int height{};
    };

} // namespace re


#endif //RAVENENGINE_CONFIG_HPP
