#pragma once
#include "FireCube.h"

class TransformGizmo
{
public:
	virtual ~TransformGizmo() {};
	virtual void SetPosition(FireCube::vec3 position) = 0;
	virtual void SetRotation(FireCube::mat4 rotation) = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual bool CheckOperationStart(FireCube::Scene *scene, FireCube::Node *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos) = 0;
	virtual void PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, FireCube::Node *currentNode) = 0;
	virtual void SetSnapToGrid(bool snap) = 0;
};