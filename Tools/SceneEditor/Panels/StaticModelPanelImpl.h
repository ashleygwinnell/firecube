#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class StaticModelDescriptor;

class StaticModelPanelImpl : public StaticModelPanel, PanelCommon<StaticModelDescriptor>
{
	FIRECUBE_OBJECT(StaticModelPanelImpl);
public:
	StaticModelPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~StaticModelPanelImpl();
private:
	virtual void FileChanged(wxFileDirPickerEvent& event);
	virtual void CastShadowChanged(wxCommandEvent& event);	
	virtual void UpdateUI() override;

	BaseComponentPanelImpl *parent;	
};