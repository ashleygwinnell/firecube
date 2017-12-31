#pragma once

#include "FireCube.h"

class NodeDescriptor;
class EditorState;
class Command;

class AuxRenderWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(AuxRenderWindow);
public:
	AuxRenderWindow(FireCube::Engine *engine);
	void Render();
	void Reset();
	FireCube::Node *GetRoot();
private:
	void CreateGrid(float size, unsigned int numberOfCells);
	void HandleInput(float t, const FireCube::MappedInput &input);

	FireCube::Scene *scene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *cameraTarget, *clientRoot;

	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::SharedPtr<FireCube::SceneView> sceneView;
	FireCube::SharedPtr<FireCube::RenderSurface> renderSurface;
	bool mouseOverView;
	FireCube::vec2 canvasPos;
	FireCube::vec2 canvasSize;
};