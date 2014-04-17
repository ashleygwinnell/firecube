#pragma once

#include <string>

namespace FireCube
{

class StringHash
{
public:
	StringHash();
	StringHash(const std::string &str);
	StringHash(const char *str);
	bool operator == (const StringHash &rhs) const {return hash == rhs.hash;}
	bool operator < (const StringHash &rhs) const { return hash < rhs.hash; }
private:
	unsigned int hash;
};

}