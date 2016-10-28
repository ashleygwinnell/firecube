#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class RigidBodyPanelImpl : public RigidBodyPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(RigidBodyPanelImpl);
public:
	RigidBodyPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~RigidBodyPanelImpl();
private:
	virtual void MassChanged(wxCommandEvent& event);	
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};

