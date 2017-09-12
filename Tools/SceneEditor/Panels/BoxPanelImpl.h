#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class BoxDescriptor;
class Command;

class BoxPanelImpl : public BoxPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(BoxPanelImpl);
public:
	BoxPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~BoxPanelImpl();
private:	
	virtual void CastShadowChanged(wxCommandEvent& event);	
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);
	void UpdateUI();
	void UndoPerformed(Command *command);
	
	FireCube::vec3 prevSize;	
	bool prevCastShadow;
	unsigned int prevCollisionQueryMask;
	unsigned int prevLightMask;
	
	BaseComponentPanelImpl *parent;
	Command *prevCommand;
};