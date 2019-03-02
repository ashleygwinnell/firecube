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
	void UseTranslateGizmo();
	void UseRotateGizmo();
	void UseScaleGizmo();
	bool *GetRenderGridPtr();
private:
	void CreateGrid(float size, unsigned int numberOfCells);
	void HandleInput(float t, const FireCube::MappedInput &input);
	void UpdateGizmo();
	void RenderDebugGeometry(NodeDescriptor *nodeDesc, FireCube::DebugRenderer *debugRenderer);
	void StartMaterialPick();
	void AddMesh(const std::string &path);
	void AddPrefab(const std::string &path);

	FireCube::Scene *scene;
	FireCube::Scene *editorScene;
	EditorState *editorState;
	NodeDescriptor *rootDesc;
	FireCube::Node *root;

	FireCube::Camera *defaultCamera;
	FireCube::Camera *currentCamera;
	FireCube::Node *gridNode, *editorRoot;
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
	float orbitDistance;
	bool renderGrid;
};