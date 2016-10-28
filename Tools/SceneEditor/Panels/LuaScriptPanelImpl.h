#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class LuaScriptPanelImpl : public LuaScriptPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(LuaScriptPanelImpl);
public:
	LuaScriptPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~LuaScriptPanelImpl();
private:
	virtual void FileChanged(wxFileDirPickerEvent& event);
	virtual void ObjectNameChanged(wxCommandEvent& event);
	virtual void AddPropertyClicked(wxCommandEvent& event);
	virtual void RenamePropertyClicked(wxCommandEvent& event);
	virtual void RemovePropertyClicked(wxCommandEvent& event);
	virtual void PropertyGridChanged(wxPropertyGridEvent& event);
	void PropertyGridLabelChanged(wxPropertyGridEvent& event);
	std::string GetObjectNameFromScript(const std::string &filename);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};