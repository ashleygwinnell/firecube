#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class CameraPanelImpl : public CameraPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(CameraPanelImpl);
public:
	CameraPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~CameraPanelImpl();
private:

	void UpdatePanelsVisibility(bool orthographic);
	void UpdateUI();

	BaseComponentPanelImpl *parent;
protected:

	virtual void ProjectionTypeChanged(wxCommandEvent& event) override;
	virtual void ZNearChanged(wxCommandEvent& event) override;
	virtual void ZFarChanged(wxCommandEvent& event) override;
	virtual void LeftChanged(wxCommandEvent& event) override;
	virtual void RightChanged(wxCommandEvent& event) override;
	virtual void TopChanged(wxCommandEvent& event) override;
	virtual void BottomChanged(wxCommandEvent& event) override;
	virtual void FovChanged(wxCommandEvent& event) override;

};