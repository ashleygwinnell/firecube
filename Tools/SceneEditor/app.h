#include "EditorState.h"
class MainFrameImpl;

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
	FireCube::Scene *GetEditorScene();
	EditorState *GetEditorState();
	void InitScene();
	MainFrameImpl *GetMainFrame();
	FireCubeApp fcApp;	
	
private:	
	MainFrameImpl *frame;
	FireCube::Scene *scene;
	FireCube::Scene *editorScene;
	EditorState *editorState;		
};