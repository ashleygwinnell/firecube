#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class RigidBodyPanelImpl : public RigidBodyPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(RigidBodyPanelImpl);
public:
	RigidBodyPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~RigidBodyPanelImpl();
private:
	void UndoPerformed(Command *command);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
	float prevMass;
	Command *prevCommand;
};

