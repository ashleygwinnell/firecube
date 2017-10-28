#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class PlaneDescriptor;

class PlanePanelImpl : public PlanePanel, public PanelCommon<PlaneDescriptor>
{
	FIRECUBE_OBJECT(PlanePanelImpl);
public:
	PlanePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~PlanePanelImpl();
private:
	virtual void CastShadowChanged(wxCommandEvent& event) override;
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event) override;	
	virtual void UpdateUI() override;

	BaseComponentPanelImpl *parent;	
};