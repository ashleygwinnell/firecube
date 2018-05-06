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
	auto i = std::find_if(properties.begin(), properties.end(), [name](const std::pair<std::string, std::string> &property) {
		return property.first == name;
	});

	if (i == properties.end())
	{
		properties.push_back(std::make_pair(name, value));
	}
	else
	{
		i->second = value;
	}
}

void LuaScriptDescriptor::RemoveProperty(const std::string &name)
{
	auto newEnd = std::remove_if(properties.begin(), properties.end(), [name](const std::pair<std::string, std::string> &property) { 
		return property.first == name;
	});

	properties.erase(newEnd, properties.end());	
}

void LuaScriptDescriptor::ClearProperties()
{
	properties.clear();
}

std::vector<std::pair<std::string, std::string>> &LuaScriptDescriptor::GetProperties()
{
	return properties;
}

std::string LuaScriptDescriptor::GetProperty(const std::string &name) const
{
	auto i = std::find_if(properties.begin(), properties.end(), [name](const std::pair<std::string, std::string> &property) {
		return property.first == name;
	});

	if (i == properties.end())
	{
		return "";
	}
	else
	{
		return i->second;
	}
}

void LuaScriptDescriptor::RenameProperty(int index, const std::string &newName)
{
	properties[index].first = newName;
}

bool LuaScriptDescriptor::HasProperty(const std::string &name) const
{
	return std::find_if(properties.begin(), properties.end(), [name](const std::pair<std::string, std::string> &prop) {
		return prop.first == name;
	}) != properties.end();
}

void LuaScriptDescriptor::SetProperties(const std::vector<std::pair<std::string, std::string>> &properties)
{
	this->properties = properties;
}
