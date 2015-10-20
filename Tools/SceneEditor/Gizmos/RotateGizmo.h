#pragma once

#include "FireCube.h"
#include "TransformGizmo.h"

class RotateGizmo : public FireCube::Object, public TransformGizmo
{
	FIRECUBE_OBJECT(RotateGizmo)
public:
	RotateGizmo(FireCube::Engine *engine, FireCube::Node *parent);
	virtual void SetPosition(FireCube::vec3 position);
	virtual void SetRotation(FireCube::mat4 rotation);
	virtual void SetScale(float scale);
	virtual void Show();
	virtual void Hide();
	virtual bool CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos);
	virtual void PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, NodeDescriptor *currentNode);
	virtual void SetSnapToGrid(bool snap);
	virtual Command *GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc);
private:
	FireCube::Node *node;	
	std::string currentAxis;	
	FireCube::vec3 startRotation, endRotation;
	bool snapToGrid;
	FireCube::vec2 lastMousePos;
};