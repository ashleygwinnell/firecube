#include "StringHash.h"
#include "Math/MathUtils.h"

using namespace FireCube;

StringHash::StringHash() : hash(0)
{

}

StringHash::StringHash(const std::string &str) : StringHash(str.c_str())
{
	
}

StringHash::StringHash(const char *str)
{
	hash = MathUtils::MurmurHash2(str, strlen(str), 0x1234);
}

unsigned int FireCube::StringHash::Value() const
{
	return hash;
}
