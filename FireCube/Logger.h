#ifndef LOGGER_H
#define LOGGER_H

namespace FireCube
{
namespace Logger
{
enum LogLevel
{
    LOG_DEBUG, LOG_INFO, LOG_TRACE, LOG_WARNING, LOG_ERROR
};
void FIRECUBE_API Init(std::string filename);
void FIRECUBE_API Write(LogLevel level, std::string str);
void FIRECUBE_API SetLogLevel(LogLevel level);
void FIRECUBE_API SetLogOutput(bool file, bool con);
void FIRECUBE_API Close();
}
}
#endif