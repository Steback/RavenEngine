#ifndef RAVENENGINE_LOGS_HPP
#define RAVENENGINE_LOGS_HPP


#include "spdlog/spdlog.h"

#include "Logger.hpp"

/**
 * @brief Default error log macro
 */
#define RE_ERROR_LOG(message) re::Logger::getFile("error.log")->error(message);


#endif //RAVENENGINE_LOGS_HPP
