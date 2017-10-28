#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class CollisionShapeDescriptor;

class CollisionShapePanelImpl : public CollisionShapePanel, public PanelCommon<CollisionShapeDescriptor>
{
	FIRECUBE_OBJECT(CollisionShapePanelImpl);
public:
	CollisionShapePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~CollisionShapePanelImpl();
private:
	virtual void UpdateUI() override;
	void UpdateVisibility(FireCube::CollisionShapeType type);
	virtual void ShapeTypeChanged(wxCommandEvent& event) override;
	virtual void TriggerChanged(wxCommandEvent& event) override;
	virtual void MeshFileChanged(wxFileDirPickerEvent& event) override;

	BaseComponentPanelImpl *parent;
};