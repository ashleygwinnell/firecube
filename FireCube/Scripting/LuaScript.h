#pragma once

#include "Utils/utils.h"
#include "Core/Component.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"

namespace FireCube
{

class LuaFile;

class FIRECUBE_API LuaScript : public Component
{
	OBJECT(LuaScript);
public:
	LuaScript(Engine *engine);
	void CreateObject(const std::string &objectName);
	void CreateObject(LuaFile *luaFile, const std::string &objectName);
private:
	void Update(float time);
	
	virtual void MarkedDirty();
	virtual void NodeChanged();

	luabridge::LuaRef object;
	luabridge::LuaRef initFunction, updateFunction;
};

}