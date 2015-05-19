#pragma once

#include "mainframe.h"

class BaseComponentPanelImpl;

class CharacterControllerPanelImpl : public CharacterControllerPanel
{
public:
	CharacterControllerPanelImpl(BaseComponentPanelImpl* parent);
	~CharacterControllerPanelImpl();
private:	
	void RemoveComponentClicked(wxCommandEvent& event);
	virtual void RadiusXChanged(wxCommandEvent& event);
	virtual void RadiusYChanged(wxCommandEvent& event);
	virtual void RadiusZChanged(wxCommandEvent& event);
	
	BaseComponentPanelImpl *parent;
};