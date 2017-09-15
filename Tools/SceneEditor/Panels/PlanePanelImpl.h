#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class PlanePanelImpl : public PlanePanel, public FireCube::Object
{
	FIRECUBE_OBJECT(PlanePanelImpl);
public:
	PlanePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~PlanePanelImpl();
private:
	virtual void CastShadowChanged(wxCommandEvent& event) override;
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event) override;
	void UndoPerformed(Command *command);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
	unsigned int prevUIntVal;
	FireCube::vec2 prevSize;
	Command *prevCommand;
};