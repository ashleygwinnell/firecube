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
}
