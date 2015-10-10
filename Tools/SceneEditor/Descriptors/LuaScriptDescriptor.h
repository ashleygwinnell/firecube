#pragma once

#include "ComponentDescriptor.h"

class LuaScriptDescriptor : public ComponentDescriptor
{
public:
	LuaScriptDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~LuaScriptDescriptor();

	void SetScriptFilename(const std::string &scriptFilename);
	std::string GetScriptFilename();
	void SetObjectName(const std::string &objectName);
	std::string GetObjectName() const;

	void SetProperty(const std::string &name, const std::string &value);
	void RemoveProperty(const std::string &name);
	void ClearProperties();
	std::map<std::string, std::string> &GetProperties();
	std::string GetProperty(const std::string &name) const;
private:
	std::string scriptFilename;
	std::string objectName;
	std::map<std::string, std::string> properties;
	
};