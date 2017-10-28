#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class RigidBodyDescriptor;

class RigidBodyPanelImpl : public RigidBodyPanel, PanelCommon<RigidBodyDescriptor>
{
	FIRECUBE_OBJECT(RigidBodyPanelImpl);
public:
	RigidBodyPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~RigidBodyPanelImpl();
private:
	void UndoPerformed(Command *command);
	virtual void UpdateUI() override;

	BaseComponentPanelImpl *parent;	
};

