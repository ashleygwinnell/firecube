#pragma once

#include "Utils/utils.h"
#include <sstream>

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
#define LOGDEBUG1(m0) { std::ostringstream oss; oss << m0; Logger::Write(Logger::LOG_DEBUG, oss.str());}
#define LOGINFO1(m0)  { std::ostringstream oss; oss << m0; Logger::Write(Logger::LOG_INFO, oss.str());}
#define LOGTRACE1(m0) { std::ostringstream oss; oss << m0; Logger::Write(Logger::LOG_TRACE, oss.str());}
#define LOGWARNING1(m0) { std::ostringstream oss; oss << m0; Logger::Write(Logger::LOG_WARNING, oss.str());}
#define LOGERROR1(m0) { std::ostringstream oss; oss << m0; Logger::Write(Logger::LOG_ERROR, oss.str());}

#define LOGDEBUG2(m0, m1) { std::ostringstream oss; oss << m0 << m1; Logger::Write(Logger::LOG_DEBUG, oss.str());}
#define LOGINFO2(m0, m1) { std::ostringstream oss; oss << m0 << m1; Logger::Write(Logger::LOG_INFO, oss.str());}
#define LOGTRACE2(m0, m1) { std::ostringstream oss; oss << m0 << m1; Logger::Write(Logger::LOG_TRACE, oss.str());}
#define LOGWARNING2(m0, m1) { std::ostringstream oss; oss << m0 << m1; Logger::Write(Logger::LOG_WARNING, oss.str());}
#define LOGERROR2(m0, m1) { std::ostringstream oss; oss << m0 << m1; Logger::Write(Logger::LOG_ERROR, oss.str());}

#define LOGDEBUG3(m0, m1, m2) { std::ostringstream oss; oss << m0 << m1 << m2; Logger::Write(Logger::LOG_DEBUG, oss.str());}
#define LOGINFO3(m0, m1, m2) { std::ostringstream oss; oss << m0 << m1 << m2; Logger::Write(Logger::LOG_INFO, oss.str());}
#define LOGTRACE3(m0, m1, m2) { std::ostringstream oss; oss << m0 << m1 << m2; Logger::Write(Logger::LOG_TRACE, oss.str());}
#define LOGWARNING3(m0, m1, m2) { std::ostringstream oss; oss << m0 << m1 << m2; Logger::Write(Logger::LOG_WARNING, oss.str());}
#define LOGERROR3(m0, m1, m2) { std::ostringstream oss; oss << m0 << m1 << m2; Logger::Write(Logger::LOG_ERROR, oss.str());}

#define LOGDEBUG4(m0, m1, m2, m3) { std::ostringstream oss; oss << m0 << m1 << m2 << m3; Logger::Write(Logger::LOG_DEBUG, oss.str());}
#define LOGINFO4(m0, m1, m2, m3) { std::ostringstream oss; oss << m0 << m1 << m2 << m3; Logger::Write(Logger::LOG_INFO, oss.str());}
#define LOGTRACE4(m0, m1, m2, m3) { std::ostringstream oss; oss << m0 << m1 << m2 << m3; Logger::Write(Logger::LOG_TRACE, oss.str());}
#define LOGWARNING4(m0, m1, m2, m3) { std::ostringstream oss; oss << m0 << m1 << m2 << m3; Logger::Write(Logger::LOG_WARNING, oss.str());}
#define LOGERROR4(m0, m1, m2, m3) { std::ostringstream oss; oss << m0 << m1 << m2 << m3; Logger::Write(Logger::LOG_ERROR, oss.str());}


#define COUNT_LOG_ARGS_IMPL2(_1, _2, _3, _4, count, ...) count
#define COUNT_LOG_ARGS_IMPL(args) COUNT_LOG_ARGS_IMPL2 args
#define COUNT_LOG_ARGS(...) COUNT_LOG_ARGS_IMPL((__VA_ARGS__, 4, 3, 2, 1, 0))
#define LOG_CHOOSE_HELPER2(name, count) name##count
#define LOG_CHOOSE_HELPER1(name, count) LOG_CHOOSE_HELPER2(name, count)
#define LOG_CHOOSE_HELPER(name, count) LOG_CHOOSE_HELPER1(name, count)
#define LOG_GLUE(x, y) x y

#define LOGDEBUG(...) LOG_GLUE(LOG_CHOOSE_HELPER(LOGDEBUG, COUNT_LOG_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define LOGINFO(...) LOG_GLUE(LOG_CHOOSE_HELPER(LOGINFO, COUNT_LOG_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define LOGTRACE(...) LOG_GLUE(LOG_CHOOSE_HELPER(LOGTRACE, COUNT_LOG_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define LOGWARNING(...) LOG_GLUE(LOG_CHOOSE_HELPER(LOGWARNING, COUNT_LOG_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define LOGERROR(...) LOG_GLUE(LOG_CHOOSE_HELPER(LOGERROR, COUNT_LOG_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#else
#define LOGDEBUG(...)
#define LOGINFO(...)
#define LOGTRACE(...)
#define LOGWARNING(...)
#define LOGERROR(...)
#endif

}
