#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class SpherePanelImpl : public SpherePanel
{
public:
	SpherePanelImpl(BaseComponentPanelImpl* parent);
	~SpherePanelImpl();
private:
	virtual void RadiusChanged(wxCommandEvent& event);
	virtual void ColumnsChanged(wxCommandEvent& event);
	virtual void RingsChanged(wxCommandEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	virtual void LightMaskChanged(wxCommandEvent& event);
	virtual void CollisionQueryMaskChanged(wxCommandEvent& event);
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);

	BaseComponentPanelImpl *parent;
};