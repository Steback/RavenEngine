#ifndef RAVENENGINE_LOGS_HPP
#define RAVENENGINE_LOGS_HPP


#include "Logger.hpp"


#define RE_ERROR_LOG(message) re::Logger::getFile("error.log")->error(message);


#endif //RAVENENGINE_LOGS_HPP
