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
	~MyApp();
	virtual bool OnInit();
	FireCube::Scene *GetScene();
	EditorState *GetEditorState();
	void InitScene();
	FireCubeApp fcApp;
private:	
	MainFrameImpl *frame;
	FireCube::Scene *scene;
	EditorState *editorState;
};