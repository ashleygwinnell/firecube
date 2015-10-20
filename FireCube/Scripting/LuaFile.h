#pragma once

#include "Utils/utils.h"
#include "Core/Resource.h"

namespace FireCube
{

class Engine;

class FIRECUBE_API LuaFile : public Resource
{
	FIRECUBE_OBJECT(LuaFile);
public:
	LuaFile(Engine *engine);
	virtual bool Load(const std::string &filename);
	const std::string &GetCode() const;
	bool HasExecuted() const;
	void SetExecuted(bool executed);
private:
	std::string code;	
	bool hasExecuted;
};

}