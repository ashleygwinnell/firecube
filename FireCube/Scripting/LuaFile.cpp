#include <string>
#include <fstream>
#include <streambuf>
#include "Scripting/LuaFile.h"
#include "Utils/Filesystem.h"

using namespace FireCube;

LuaFile::LuaFile(Engine *engine) : Resource(engine), hasExecuted(false)
{

}

bool LuaFile::Load(const std::string &filename)
{
	std::string name = Filesystem::SearchForFileName(filename);
	if (name.empty())
		return false;

	this->filename = name;
	std::ifstream t(name);
	code = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	return true;
}

const std::string &LuaFile::GetCode() const
{
	return code;
}

bool LuaFile::HasExecuted() const
{
	return hasExecuted;
}

void LuaFile::SetExecuted(bool executed)
{
	hasExecuted = executed;
}