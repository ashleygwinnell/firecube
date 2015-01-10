#pragma once

#include "FireCube.h"

class TranslateGizmo : public FireCube::Object
{
	OBJECT(TranslateGizmo)
public:
	TranslateGizmo(FireCube::Engine *engine, FireCube::Node *parent);
	void SetPosition(FireCube::vec3 position);
	void Show();
	void Hide();
	bool CheckOperationStart(FireCube::Scene *scene, FireCube::Node *currentNode, FireCube::Ray ray);	
	void PerformOperation(FireCube::Ray ray, FireCube::Node *currentNode);
	void SetSnapToGrid(bool snap);
private:
	FireCube::Node *node;
	FireCube::vec3 startPosition, dragStart;
	std::string currentAxis;
	bool snapToGrid;
};