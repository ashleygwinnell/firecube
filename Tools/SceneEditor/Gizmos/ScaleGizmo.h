#pragma once

#include "FireCube.h"
#include "TransformGizmo.h"

class ScaleGizmo : public FireCube::Object, public TransformGizmo
{
	FIRECUBE_OBJECT(ScaleGizmo)
public:
	ScaleGizmo(FireCube::Engine *engine, FireCube::Node *parent);
	virtual void Show();
	virtual void Hide();
	virtual bool CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos);
	virtual void PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, NodeDescriptor *currentNode);
	virtual void OperationEnd();
	virtual void SetSnapToGrid(bool snap);
	virtual Command *GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc);
	virtual void UpdateTransformation(FireCube::Camera *camera, NodeDescriptor *currentNode);	

private:
	FireCube::Node *node;
	FireCube::vec3 startPosition, dragStart;
	std::string currentAxis;
	FireCube::vec3 startScale, endScale;
	bool snapToGrid;
	FireCube::vec2 startMousePos;
	FireCube::SharedPtr<FireCube::Material> xAxisMaterial, yAxisMaterial, zAxisMaterial, allAxesMaterial;
};