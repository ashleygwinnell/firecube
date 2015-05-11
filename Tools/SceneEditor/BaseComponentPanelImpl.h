#include "mainframe.h"
#include "FireCube.h"

class MyApp;

class BaseComponentPanelImpl : public BaseComponentPanel
{	
protected:
	
	MyApp *theApp;
	FireCube::Engine *engine;
	EditorState *editorState;	
	FireCube::Component *component;
public:	
	BaseComponentPanelImpl(wxWindow* parent, FireCube::Component *component);
	void SetComponent(FireCube::Component *component);
	FireCube::Component *GetComponent();
	void AddControl(wxWindow *control);
};