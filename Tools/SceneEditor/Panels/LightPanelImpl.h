#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class LightDescriptor;

class LightPanelImpl : public LightPanel, public PanelCommon<LightDescriptor>
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
	virtual void UpdateUI() override;

	BaseComponentPanelImpl *parent;	
};