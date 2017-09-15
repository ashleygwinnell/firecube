#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class CharacterControllerPanelImpl : public CharacterControllerPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(CharacterControllerPanelImpl);
public:
	CharacterControllerPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~CharacterControllerPanelImpl();
private:		
	void UpdateUI();
	void UndoPerformed(Command *command);

	BaseComponentPanelImpl *parent;
	float prevFloatVal;
	Command *prevCommand;
};