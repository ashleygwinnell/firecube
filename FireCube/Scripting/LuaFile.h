#pragma once

#include "Utils/utils.h"
#include "Core/Resource.h"

namespace FireCube
{

class Engine;

/**
* A resource representing a Lua script file
*/
class FIRECUBE_API LuaFile : public Resource
{
	FIRECUBE_OBJECT(LuaFile);
public:
	LuaFile(Engine *engine);
	
	/**
	* Loads the script for a file
	* @param filename The file to load
	*/	
	virtual bool Load(const std::string &filename);
	const std::string &GetCode() const;
	bool HasExecuted() const;
	void SetExecuted(bool executed);
private:
	std::string code;	
	bool hasExecuted;
};

}