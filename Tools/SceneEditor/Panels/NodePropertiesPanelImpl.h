#pragma once

#include "../mainframe.h"
#include "FireCube.h"

class EditorState;
class NodeDescriptor;
class Command;

class NodePropertiesPanelImpl : public NodePropertiesPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(NodePropertiesPanelImpl)
public:
	NodePropertiesPanelImpl(wxWindow* parent);
	~NodePropertiesPanelImpl();

	virtual void PositionXChanged(wxCommandEvent& event);
	virtual void TextSetFocus(wxFocusEvent& event) override;
	virtual void TextKillFocus(wxFocusEvent& event) override;
	virtual void PositionYChanged(wxCommandEvent& event);
	virtual void PositionZChanged(wxCommandEvent& event);
	virtual void RotationXChanged(wxCommandEvent& event);
	virtual void RotationYChanged(wxCommandEvent& event);
	virtual void RotationZChanged(wxCommandEvent& event);
	virtual void ScaleXChanged(wxCommandEvent& event);
	virtual void ScaleYChanged(wxCommandEvent& event);
	virtual void ScaleZChanged(wxCommandEvent& event);
	virtual void NameChanged(wxCommandEvent& event);
	void NodeRenamed(NodeDescriptor *node);
	void UndoPerformed(Command *command);
private:
	void UpdateUI();

	EditorState *editorState;
	FireCube::vec3 prevTranslation;
	FireCube::vec3 prevRotation;
	FireCube::vec3 prevScale;
	bool commandAdded;

};