#include "EditorState.h"
#include "SceneSettings.h"

class FireCubeApp : public FireCube::Application
{
public:
	void Render(float t) {};
	void Update(float t) {};

};

class MyApp : public wxApp
{
public:
	~MyApp();
	virtual bool OnInit();
	FireCube::Scene *GetScene();
	EditorState *GetEditorState();
	SceneSettings *GetSceneSettings();
	void InitScene();
	FireCubeApp fcApp;
private:	
	MainFrameImpl *frame;
	FireCube::Scene *scene;
	EditorState *editorState;
	SceneSettings sceneSettings;
};