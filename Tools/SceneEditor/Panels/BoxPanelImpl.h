#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class BoxPanelImpl : public BoxPanel
{
public:
	BoxPanelImpl(BaseComponentPanelImpl* parent);
	~BoxPanelImpl();
private:
	virtual void WidthChanged(wxCommandEvent& event);
	virtual void HeightChanged(wxCommandEvent& event);
	virtual void DepthChanged(wxCommandEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	virtual void LightMaskChanged(wxCommandEvent& event);
	virtual void CollisionQueryMaskChanged(wxCommandEvent& event);
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);
	
	BaseComponentPanelImpl *parent;
};