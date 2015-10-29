#pragma once

#include "../mainframe.h"
#include "FireCube.h"

class EditorState;
//class NodeDescriptor;

class MaterialEditorPanelImpl : public MaterialEditorPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(MaterialEditorPanelImpl)
public:
	MaterialEditorPanelImpl(wxWindow* parent);
	~MaterialEditorPanelImpl();	

private:

	virtual void NewButtonClicked(wxCommandEvent& event);
	virtual void OpenButtonClicked(wxCommandEvent& event);
	virtual void SaveButtonClicked(wxCommandEvent& event);
	virtual void SaveAsButtonClicked(wxCommandEvent& event);
	virtual void PropertyGridChanged(wxPropertyGridEvent& event);

	void FillPropertyGrid(FireCube::Material *material);
	EditorState *editorState;
	std::string currentFileName;
	FireCube::SharedPtr<FireCube::Material> material;
};