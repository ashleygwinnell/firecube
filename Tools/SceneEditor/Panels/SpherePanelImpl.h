#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class SpherePanelImpl : public SpherePanel, public FireCube::Object
{
	FIRECUBE_OBJECT(SpherePanelImpl)
public:
	SpherePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~SpherePanelImpl();
private:	
	virtual void CastShadowChanged(wxCommandEvent& event) override;
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event) override;
	void UndoPerformed(Command *command);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
	unsigned int prevUIntVal;
	float prevFloatVal;
	Command *prevCommand;
	bool skipUiUpdate;
};