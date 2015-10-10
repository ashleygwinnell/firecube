#pragma once

#include "mainframe.h"

class BaseComponentPanelImpl;

class CollisionShapePanelImpl : public CollisionShapePanel
{
public:
	CollisionShapePanelImpl(BaseComponentPanelImpl* parent);
	~CollisionShapePanelImpl();
private:
	void UpdateUI();
	void UpdateVisibility(FireCube::CollisionShapeType type);	
	virtual void ShapeTypeChanged(wxCommandEvent& event);
	virtual void PlaneXChanged(wxCommandEvent& event);
	virtual void PlaneYChanged(wxCommandEvent& event);
	virtual void PlaneZChanged(wxCommandEvent& event);
	virtual void PlaneWChanged(wxCommandEvent& event);
	virtual void BBoxMinXChanged(wxCommandEvent& event);
	virtual void BBoxMinYChanged(wxCommandEvent& event);
	virtual void BBoxMinZChanged(wxCommandEvent& event);
	virtual void BBoxMaxXChanged(wxCommandEvent& event);
	virtual void BBoxMaxYChanged(wxCommandEvent& event);
	virtual void BBoxMaxZChanged(wxCommandEvent& event);
	virtual void TriggerChanged(wxCommandEvent& event);
	virtual void MeshFileChanged(wxFileDirPickerEvent& event);
	BaseComponentPanelImpl *parent;
};