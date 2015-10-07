#include "AuxDataMap.h"

std::map<std::string, std::string> &AuxDataMap::GetMap(void *obj)
{
	auto i = maps.find(obj);
	if (i != maps.end())
	{
		return i->second;
	}
	else
	{
		maps[obj] = std::map<std::string, std::string>();
		return maps[obj];
	}
}

void AuxDataMap::Clear()
{
	maps.clear();
}