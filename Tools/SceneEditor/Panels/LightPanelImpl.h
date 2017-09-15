#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class LightPanelImpl : public LightPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(LightPanelImpl);
public:
	LightPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~LightPanelImpl();
private:	
	
	virtual void LightTypeChanged(wxCommandEvent& event) override;
	virtual void LightColorChanged(wxColourPickerEvent& event) override;
	virtual void CastShadowChanged(wxCommandEvent& event) override;	
	void UpdatePanelsVisibility(FireCube::LightType type);
	void UndoPerformed(Command *command);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
	Command *prevCommand;
	unsigned int prevMask;
	float prevFloatVal;
};