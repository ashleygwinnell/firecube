#pragma once

#include "Utils/utils.h"

namespace FireCube
{
namespace Logger
{
enum LogLevel
{
    LOG_DEBUG, LOG_INFO, LOG_TRACE, LOG_WARNING, LOG_ERROR
};
void FIRECUBE_API Init(const std::string &filename);
void FIRECUBE_API Write(LogLevel level, const std::string &str);
void FIRECUBE_API SetLogLevel(LogLevel level);
void FIRECUBE_API SetLogOutput(bool file, bool con);
void FIRECUBE_API Close();
}

#ifdef ENABLE_LOGGING
#define LOGDEBUG(message) Logger::Write(Logger::LOG_DEBUG, message)
#define LOGINFO(message) Logger::Write(Logger::LOG_INFO, message)
#define LOGTRACE(message) Logger::Write(Logger::LOG_TRACE, message)
#define LOGWARNING(message) Logger::Write(Logger::LOG_WARNING, message)
#define LOGERROR(message) Logger::Write(Logger::LOG_ERROR, message)
#else
#define LOGDEBUG(message)
#define LOGINFO(message)
#define LOGTRACE(message)
#define LOGWARNING(message)
#define LOGERROR(message)
#endif

}
