#pragma once

#include <map>
#include <string>

class AuxDataMap
{
public:
	std::map<std::string, std::string> &GetMap(void *obj);
	void Clear();
private:
	std::map<void *, std::map<std::string, std::string>> maps;
};