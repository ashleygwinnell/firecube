#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <iostream>
using namespace std;

#include "Utils/utils.h"
#include "Utils/Logger.h"

using namespace FireCube;

ofstream file;
Logger::LogLevel minLevel = Logger::LOG_DEBUG;
bool outputToFile = true, outputToCout = true;

char* levelNames[] = {"[DEBUG]", "[INFO]", "[TRACE]", "[WARNING]", "[ERROR]"};

void Logger::Init(const string &filename)
{
    file.open(filename.c_str(), ios::out | ios::trunc);
}

void Logger::Write(LogLevel level, const string &str)
{
    if (level < minLevel)
        return;
    if (outputToFile)
    {
        if (file.is_open())
            file << levelNames[level] << ": " << str << endl;
    }
    if (outputToCout)
        cout << levelNames[level] << ": " << str << endl;
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
