#pragma once

#include "../mainframe.h"
#include "FireCube.h"
#include <wx/dnd.h>

class EditorState;
class MaterialEditorPanelImpl;

class MaterialEditorDropTarget : public wxDropTarget
{
public:
	MaterialEditorDropTarget(MaterialEditorPanelImpl *materialEditorPanel);

	virtual wxDragResult OnData(wxCoord vX, wxCoord vY, wxDragResult eResult) override;
	virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def) override;
	virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def) override;

private:
	MaterialEditorPanelImpl *materialEditorPanel;
};

class MaterialEditorPanelImpl : public MaterialEditorPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(MaterialEditorPanelImpl)
		
	friend class MaterialEditorDropTarget;
public:
	MaterialEditorPanelImpl(wxWindow* parent);
	~MaterialEditorPanelImpl();	

private:

	virtual void NewButtonClicked(wxCommandEvent& event);
	virtual void OpenButtonClicked(wxCommandEvent& event);
	virtual void SaveButtonClicked(wxCommandEvent& event);
	virtual void SaveAsButtonClicked(wxCommandEvent& event);
	virtual void PickMaterialButtonClicked(wxCommandEvent& event);
	virtual void PropertyGridChanged(wxPropertyGridEvent& event);

	void MaterialPicked(FireCube::Material *material);
	void FillPropertyGrid(FireCube::Material *material);
	EditorState *editorState;
	std::string currentFileName;
	FireCube::SharedPtr<FireCube::Material> material;
};
