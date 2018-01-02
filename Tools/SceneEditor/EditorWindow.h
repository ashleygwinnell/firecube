#pragma once

#include "FireCube.h"
#include "Descriptors/NodeDescriptor.h"

class TranslateGizmo;
class RotateGizmo;
class TransformGizmo;
class ScaleGizmo;
class EditorState;
class EditorCanvas;
class CameraDescriptor;

enum class Operation3
{
	NONE, OBJECT_TRANSFORM, CAMERA_ORBIT, PICK_MATERIAL
};


class EditorWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(EditorWindow);
public:
	EditorWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(FireCube::Scene *scene, NodeDescriptor *rootDesc, EditorState *editorState);
	void UseDefaultCamera();
	void UseCamera(FireCube::Camera *camera);
	void UseCamera(CameraDescriptor *camera);
	FireCube::Camera *GetCurrentCamera();
	bool IsUsingDefaultCamera() const;
private:
	void CreateGrid(float size, unsigned int numberOfCells);
	void HandleInput(float t, const FireCube::MappedInput &input);
	void UpdateGizmo();
	void SelectedNodeChanged(NodeDescriptor *nodeDesc);
	void UseTranslateGizmo();
	void UseRotateGizmo();
	void UseScaleGizmo();
	void StateChanged();

	FireCube::Scene *scene;
	FireCube::Scene *editorScene;
	EditorState *editorState;
	NodeDescriptor *rootDesc;
	FireCube::Node *root;

	FireCube::OrbitCamera *defaultCamera;
	FireCube::Camera *currentCamera;
	FireCube::Node *gridNode, *cameraTarget, *editorRoot;
	FireCube::SharedPtr<TranslateGizmo> translateGizmo;
	FireCube::SharedPtr<RotateGizmo> rotateGizmo;
	FireCube::SharedPtr<ScaleGizmo> scaleGizmo;
	TransformGizmo *transformGizmo;
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
	Operation3 currentOperation;
	FireCube::Timer timer;
	FireCube::SharedPtr<FireCube::RenderSurface> renderSurface;
	bool mouseOverView;
	FireCube::vec2 canvasPos, canvasSize;
	bool leftButtonDown;
	bool firstLeftDownOutside;
};