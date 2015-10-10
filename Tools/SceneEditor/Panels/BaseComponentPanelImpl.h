#include "../mainframe.h"
#include "FireCube.h"
#include "../Descriptors/ComponentDescriptor.h"

class MyApp;
class ComponentDescriptor;

class BaseComponentPanelImpl : public BaseComponentPanel
{	
protected:
	virtual void RemoveComponentClicked(wxCommandEvent& event);

	MyApp *theApp;
	FireCube::Engine *engine;
	EditorState *editorState;	
	ComponentDescriptor *componentDesc;
public:	
	BaseComponentPanelImpl(wxWindow* parent, ComponentDescriptor *componentDesc);
	void SetComponent(ComponentDescriptor *componentDesc);
	ComponentDescriptor *GetComponent();
	void AddControl(wxWindow *control);
};