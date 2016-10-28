#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class StaticModelPanelImpl : public StaticModelPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(StaticModelPanelImpl);
public:
	StaticModelPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~StaticModelPanelImpl();
private:
	virtual void FileChanged(wxFileDirPickerEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);
	virtual void LightMaskChanged(wxCommandEvent& event);
	virtual void CollisionQueryMaskChanged(wxCommandEvent& event);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
};