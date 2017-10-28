#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class BoxDescriptor;

class BoxPanelImpl : public BoxPanel, public PanelCommon<BoxDescriptor>
{
	FIRECUBE_OBJECT(BoxPanelImpl);
public:
	BoxPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~BoxPanelImpl();
private:	
	virtual void CastShadowChanged(wxCommandEvent& event);	
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);
	virtual void UpdateUI() override;
	
	BaseComponentPanelImpl *parent;
};