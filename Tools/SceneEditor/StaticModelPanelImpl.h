#pragma once

#include "mainframe.h"

class BaseComponentPanelImpl;

class StaticModelPanelImpl : public StaticModelPanel
{
public:
	StaticModelPanelImpl(BaseComponentPanelImpl* parent);
	~StaticModelPanelImpl();
private:
	virtual void FileChanged(wxFileDirPickerEvent& event);
	void RemoveComponentClicked(wxCommandEvent& event);

	BaseComponentPanelImpl *parent;
};