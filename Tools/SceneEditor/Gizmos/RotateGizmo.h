#pragma once

#include "FireCube.h"
#include "TransformGizmo.h"

class RotateGizmo : public FireCube::Object, public TransformGizmo
{
	FIRECUBE_OBJECT(RotateGizmo)
public:
	RotateGizmo(FireCube::Engine *engine, FireCube::Node *parent);
	virtual void Show();
	virtual void Hide();
	virtual bool CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos);
	virtual void PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, NodeDescriptor *currentNode);
	virtual void SetSnapToGrid(bool snap);
	virtual Command *GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc);
	virtual void UpdateTransformation(FireCube::Camera *camera, NodeDescriptor *currentNode);
	virtual void OperationEnd();
private:
	FireCube::Geometry *CreateArc(float radius, float startAngle, float endAngle, unsigned int tesselation);
	FireCube::Node *node;	
	std::string currentAxis;	
	FireCube::vec3 startRotation, endRotation;
	bool snapToGrid;	
	FireCube::Node *xAxis, *yAxis, *zAxis;
	FireCube::Plane currentPlane;
	FireCube::vec3 planeStartPoint;
	FireCube::mat4 startRotationMatrix;
	FireCube::mat4 parentRotationMatrix;
	FireCube::SharedPtr<FireCube::Material> xAxisMaterial, yAxisMaterial, zAxisMaterial;
};