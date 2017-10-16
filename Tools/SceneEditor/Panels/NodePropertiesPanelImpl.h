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

	virtual void NameChanged(wxCommandEvent& event);
	void NodeRenamed(NodeDescriptor *node);
	void UndoPerformed(Command *command);
private:
	void UpdateUI();

	EditorState *editorState;
	FireCube::vec3 prevTranslation;
	FireCube::vec3 prevRotation;
	FireCube::vec3 prevScale;
	Command *prevCommand;
	bool skipUiUpdate;

};