#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class SpherePanelImpl : public SpherePanel, public FireCube::Object
{
	FIRECUBE_OBJECT(SpherePanelImpl)
public:
	SpherePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~SpherePanelImpl();
private:
	virtual void RadiusChanged(wxCommandEvent& event);
	virtual void ColumnsChanged(wxCommandEvent& event);
	virtual void RingsChanged(wxCommandEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	virtual void LightMaskChanged(wxCommandEvent& event);
	virtual void CollisionQueryMaskChanged(wxCommandEvent& event);
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};