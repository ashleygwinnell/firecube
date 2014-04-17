#include "StringHash.h"
#include "Math/MathUtils.h"

using namespace FireCube;

StringHash::StringHash()
{

}

StringHash::StringHash(const std::string &str)
{
	hash = MathUtils::MurmurHash2(str.c_str(), str.size(), 0x1234);
}

StringHash::StringHash(const char *str)
{
	hash = MathUtils::MurmurHash2(str, strlen(str), 0x1234);
}