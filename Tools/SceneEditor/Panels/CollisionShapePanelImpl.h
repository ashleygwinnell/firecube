#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class CollisionShapePanelImpl : public CollisionShapePanel, public FireCube::Object
{
	FIRECUBE_OBJECT(CollisionShapePanelImpl);
public:
	CollisionShapePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~CollisionShapePanelImpl();
private:
	void UpdateUI();
	void UpdateVisibility(FireCube::CollisionShapeType type);	
	virtual void ShapeTypeChanged(wxCommandEvent& event) override;		
	virtual void TriggerChanged(wxCommandEvent& event) override;
	virtual void MeshFileChanged(wxFileDirPickerEvent& event) override;
	void UndoPerformed(Command *command);

	BaseComponentPanelImpl *parent;
	Command *prevCommand;	
	FireCube::Plane prevPlane;
	FireCube::BoundingBox prevBoundingBox;
	float prevRadius;
	bool skipUiUpdate;
};