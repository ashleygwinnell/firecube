#pragma once

#include "FireCube.h"
#include "TransformGizmo.h"

class TranslateGizmo : public FireCube::Object, public TransformGizmo
{
	OBJECT(TranslateGizmo)
public:
	TranslateGizmo(FireCube::Engine *engine, FireCube::Node *parent);
	virtual void SetPosition(FireCube::vec3 position);
	virtual void SetRotation(FireCube::mat4 rotation);
	virtual void SetScale(float scale);
	virtual void Show();
	virtual void Hide();
	virtual bool CheckOperationStart(FireCube::Scene *scene, FireCube::Node *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos);
	virtual void PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, FireCube::Node *currentNode);
	virtual void SetSnapToGrid(bool snap);
private:
	FireCube::Node *node;
	FireCube::vec3 startPosition, dragStart;
	std::string currentAxis;
	bool snapToGrid;
};