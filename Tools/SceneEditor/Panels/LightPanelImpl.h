#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class LightPanelImpl : public LightPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(LightPanelImpl);
public:
	LightPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~LightPanelImpl();
private:	
	
	virtual void LightTypeChanged(wxCommandEvent& event);
	virtual void LightColorChanged(wxColourPickerEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	virtual void ShadowIntensityChanged(wxCommandEvent& event);
	virtual void RangeChanged(wxCommandEvent& event);
	virtual void SpotCutoffChanged(wxCommandEvent& event);
	virtual void MaskChanged(wxCommandEvent& event);
	void UpdatePanelsVisibility(FireCube::LightType type);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};