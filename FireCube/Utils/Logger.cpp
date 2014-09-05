#include <fstream>
#include <iostream>

#include "Utils/Logger.h"

using namespace FireCube;

std::ofstream file;
Logger::LogLevel minLevel = Logger::LogLevel::LEVEL_DEBUG;
bool outputToFile = true, outputToCout = true;

char* levelNames[] = {"[DEBUG]", "[INFO]", "[TRACE]", "[WARNING]", "[ERROR]"};

void Logger::Init(const std::string &filename)
{
    file.open(filename.c_str(), std::ios::out | std::ios::trunc);
}

void Logger::Write(LogLevel level, const std::string &str)
{
    if (level < minLevel)
        return;
    if (outputToFile)
    {
        if (file.is_open())
            file << levelNames[static_cast<int>(level)] << ": " << str << std::endl;
    }
    if (outputToCout)
        std::cout << levelNames[static_cast<int>(level)] << ": " << str << std::endl;
}

void FIRECUBE_API Logger::SetLogLevel(Logger::LogLevel level)
{
    minLevel = level;
}

void FIRECUBE_API Logger::SetLogOutput(bool file, bool con)
{
    outputToFile = file;
    outputToCout = con;
}

void Logger::Close()
{
    file.close();
}
