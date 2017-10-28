#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class SphereDescriptor;

class SpherePanelImpl : public SpherePanel, public PanelCommon<SphereDescriptor>
{
	FIRECUBE_OBJECT(SpherePanelImpl)
public:
	SpherePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~SpherePanelImpl();
private:	
	virtual void CastShadowChanged(wxCommandEvent& event) override;
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event) override;	
	virtual void UpdateUI() override;

	BaseComponentPanelImpl *parent;	
};