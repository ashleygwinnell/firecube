#include "SceneDescriptor.h"
#include "EditorState.h"

class FireCubeApp : public FireCube::Application
{
public:
	void Render(float t) {};
	void Update(float t) {};

};

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	SceneDescriptor &GetSceneDescriptor();
	EditorState *GetEditorState();

	FireCubeApp fcApp;
private:	
	MainFrameImpl *frame;
	SceneDescriptor sceneDescriptor;
	EditorState *editorState;
};