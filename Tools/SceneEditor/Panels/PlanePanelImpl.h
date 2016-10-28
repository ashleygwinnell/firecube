#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class PlanePanelImpl : public PlanePanel, public FireCube::Object
{
	FIRECUBE_OBJECT(PlanePanelImpl);
public:
	PlanePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~PlanePanelImpl();
private:
	virtual void WidthChanged(wxCommandEvent& event);	
	virtual void DepthChanged(wxCommandEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	virtual void LightMaskChanged(wxCommandEvent& event);
	virtual void CollisionQueryMaskChanged(wxCommandEvent& event);
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};