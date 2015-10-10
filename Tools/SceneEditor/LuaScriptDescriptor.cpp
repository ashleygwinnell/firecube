#include "LuaScriptDescriptor.h"

using namespace FireCube;

LuaScriptDescriptor::LuaScriptDescriptor(): ComponentDescriptor(ComponentType::LUA_SCRIPT)
{

}

void LuaScriptDescriptor::CreateComponent(FireCube::Node *node, FireCube::Engine *engine)
{

}

ComponentDescriptor *LuaScriptDescriptor::Clone()
{
	auto clone = new LuaScriptDescriptor();
	clone->scriptFilename = scriptFilename;
	clone->objectName = objectName;
	clone->properties = properties;

	return clone;
}

LuaScriptDescriptor::~LuaScriptDescriptor()
{

}

void LuaScriptDescriptor::SetScriptFilename(const std::string &scriptFilename)
{
	this->scriptFilename = scriptFilename;
}

std::string LuaScriptDescriptor::GetScriptFilename()
{
	return scriptFilename;
}

void LuaScriptDescriptor::SetObjectName(const std::string &objectName)
{
	this->objectName = objectName;
}

std::string LuaScriptDescriptor::GetObjectName() const
{
	return objectName;
}

void LuaScriptDescriptor::SetProperty(const std::string &name, const std::string &value)
{
	properties[name] = value;
}

void LuaScriptDescriptor::RemoveProperty(const std::string &name)
{
	auto i = properties.find(name);
	if (i != properties.end())
	{
		properties.erase(i);
	}
}

void LuaScriptDescriptor::ClearProperties()
{
	properties.clear();
}

std::map<std::string, std::string> &LuaScriptDescriptor::GetProperties()
{
	return properties;
}

std::string LuaScriptDescriptor::GetProperty(const std::string &name) const
{
	auto i = properties.find(name);
	if (i != properties.end())
	{
		return i->second;
	}
	else
	{
		return "";
	}
}