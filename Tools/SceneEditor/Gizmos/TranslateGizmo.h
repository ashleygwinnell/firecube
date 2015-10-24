#pragma once

#include "FireCube.h"
#include "TransformGizmo.h"
class EditorState;

class TranslateGizmo : public FireCube::Object, public TransformGizmo
{
	FIRECUBE_OBJECT(TranslateGizmo)
public:
	TranslateGizmo(FireCube::Engine *engine, FireCube::Node *parent);
	virtual void Show();
	virtual void Hide();
	virtual bool CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos);
	virtual void PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, NodeDescriptor *currentNode);
	virtual void SetSnapToGrid(bool snap);
	virtual Command *GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc);
	virtual void UpdateTransformation(FireCube::Camera *camera, NodeDescriptor *currentNode);
private:
	FireCube::Node *node;
	FireCube::vec3 startPosition, dragStart, endPosition;
	std::string currentAxis;
	bool snapToGrid;	
};