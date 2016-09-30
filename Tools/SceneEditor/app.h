#include "EditorState.h"
class MainFrameImpl;
class wxGLContext;

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
	void InitEngine();
	MainFrameImpl *GetMainFrame();
	FireCubeApp fcApp;	

	void SetMainContext(wxGLContext *mainContext);
	wxGLContext *GetMainContext(wxGLCanvas *glCanvas);
	
private:	
	MainFrameImpl *frame;
	FireCube::Scene *scene;
	FireCube::Scene *editorScene;
	EditorState *editorState;	
	wxGLContext *mainContext;
	bool engineInitialized;
};