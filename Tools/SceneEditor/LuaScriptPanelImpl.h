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

	BaseComponentPanelImpl *parent;
};