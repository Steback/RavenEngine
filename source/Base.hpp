#ifndef RAVENENGINE_BASE_HPP
#define RAVENENGINE_BASE_HPP


#include <memory>

#include "utils/NonCopyable.hpp"
#include "logs/Logs.hpp"
#include "files/FilesManager.hpp"
#include "config/Config.hpp"


namespace re {

    class Renderer;

    /**
     * @brief Base class. this have all the engine's core functionalities
     */
    class Base : NonCopyable {
    public:
        /**
         * @param appName Application/Game name
         */
        explicit Base(const std::string& appName);

        ~Base() override;

        /**
         * @brief Update function for App/Game class. Will executed during the loop, before render.
         *
         * Pure virtual method, will be implemented by the Application/Game class.
         */
        virtual void update() = 0;

        /**
         * @brief Main loop, execute all update and render functionalities
         */
        void loop();

        /**
         * @brief
         */
        void run();

    private:
        Config config;
        std::unique_ptr<Renderer> renderer;
    };

} // namespace re


#endif //RAVENENGINE_BASE_HPP
