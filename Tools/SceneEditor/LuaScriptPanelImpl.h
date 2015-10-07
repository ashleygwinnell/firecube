#pragma once

#include "mainframe.h"

class BaseComponentPanelImpl;

class LuaScriptPanelImpl : public LuaScriptPanel
{
public:
	LuaScriptPanelImpl(BaseComponentPanelImpl* parent);
	~LuaScriptPanelImpl();
private:
	virtual void FileChanged(wxFileDirPickerEvent& event);
	virtual void ObjectNameChanged(wxCommandEvent& event);
	void RemoveComponentClicked(wxCommandEvent& event);
	virtual void AddPropertyClicked(wxCommandEvent& event);
	virtual void RenamePropertyClicked(wxCommandEvent& event);
	virtual void RemovePropertyClicked(wxCommandEvent& event);
	virtual void PropertyGridChanged(wxPropertyGridEvent& event);
	void PropertyGridLabelChanged(wxPropertyGridEvent& event);

	BaseComponentPanelImpl *parent;
};