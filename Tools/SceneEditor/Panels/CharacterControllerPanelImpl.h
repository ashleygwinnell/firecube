#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class CharacterControllerPanelImpl : public CharacterControllerPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(CharacterControllerPanelImpl);
public:
	CharacterControllerPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~CharacterControllerPanelImpl();
private:	

	virtual void RadiusChanged(wxCommandEvent& event);
	virtual void HeightChanged(wxCommandEvent& event);	
	virtual void ContactOffsetChanged(wxCommandEvent& event);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};