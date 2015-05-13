#pragma once

#include "mainframe.h"
#include "FireCube.h"

class EditorState;

class NodePropertiesPanelImpl : public NodePropertiesPanel, public FireCube::Object
{
	OBJECT(NodePropertiesPanelImpl)
public:
	NodePropertiesPanelImpl(wxWindow* parent);
	~NodePropertiesPanelImpl();

	virtual void PositionXChanged(wxCommandEvent& event);
	virtual void PositionYChanged(wxCommandEvent& event);
	virtual void PositionZChanged(wxCommandEvent& event);
	virtual void RotationXChanged(wxCommandEvent& event);
	virtual void RotationYChanged(wxCommandEvent& event);
	virtual void RotationZChanged(wxCommandEvent& event);
	virtual void ScaleXChanged(wxCommandEvent& event);
	virtual void ScaleYChanged(wxCommandEvent& event);
	virtual void ScaleZChanged(wxCommandEvent& event);
	virtual void NameChanged(wxCommandEvent& event);
	void NodeRenamed(FireCube::Node *node);

private:
	void UpdateUI();

	EditorState *editorState;
};