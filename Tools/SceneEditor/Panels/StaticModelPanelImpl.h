#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class StaticModelPanelImpl : public StaticModelPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(StaticModelPanelImpl);
public:
	StaticModelPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~StaticModelPanelImpl();
private:
	virtual void FileChanged(wxFileDirPickerEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	void UndoPerformed(Command *command);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
	Command *prevCommand;
	unsigned int prevUIntVal;	
};