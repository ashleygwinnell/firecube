#include "Utils/utils.h"

using namespace FireCube;

bool FireCube::IsFinite(float val)
{
    return ((val < 1e8) && (val > -1e8));
}

float FireCube::RangedRandom(float v1, float v2)
{
    return v1 + (v2 - v1) * ((float)rand()) / ((float)RAND_MAX);
}

std::string FireCube::ToLower(const std::string &str)
{
    std::string ret;
    std::string::const_iterator i = str.begin();
    for (; i != str.end(); i++)
        ret += tolower(*i);
    return ret;
}

std::string FireCube::ToUpper(const std::string &str)
{
    std::string ret;
    std::string::const_iterator i = str.begin();
    for (; i != str.end(); i++)
        ret += toupper(*i);
    return ret;
}

std::string FireCube::GetFileName(const std::string &file)
{
    std::string::size_type i = file.find_last_of("/");
    std::string::size_type i2 = file.find_last_of("\\");
    std::string::size_type maxi = 0;

    if ((i != std::string::npos) && (i2 != std::string::npos))
    {
        maxi = i > i2 ? i + 1 : i2 + 1;
    }
    else if (i != std::string::npos)
        maxi = i + 1;
    else if (i2 != std::string::npos)
        maxi = i2 + 1;

    return file.substr(maxi);
}
