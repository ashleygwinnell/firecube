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
	virtual void RadiusChanged(wxCommandEvent& event);
	virtual void HeightChanged(wxCommandEvent& event);	
	virtual void ContactOffsetChanged(wxCommandEvent& event);
	
	BaseComponentPanelImpl *parent;
};