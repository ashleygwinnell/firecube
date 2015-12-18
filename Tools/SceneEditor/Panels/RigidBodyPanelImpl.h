#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class RigidBodyPanelImpl : public RigidBodyPanel
{
public:
	RigidBodyPanelImpl(BaseComponentPanelImpl* parent);
	~RigidBodyPanelImpl();
private:
	virtual void MassChanged(wxCommandEvent& event);	

	BaseComponentPanelImpl *parent;
};

