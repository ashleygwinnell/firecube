#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class CharacterControllerDescriptor;

class CharacterControllerPanelImpl : public CharacterControllerPanel, public PanelCommon<CharacterControllerDescriptor>
{
	FIRECUBE_OBJECT(CharacterControllerPanelImpl);
public:
	CharacterControllerPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~CharacterControllerPanelImpl();
private:		
	virtual void UpdateUI() override;
	
	BaseComponentPanelImpl *parent;
};