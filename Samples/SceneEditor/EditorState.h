#pragma once

#include "FireCube.h"
#include <vector>

class NodeDescriptor;
class Command;

class EditorState : public FireCube::Object
{
	OBJECT(EditorState)
public:
	EditorState(FireCube::Engine *engine);
	void ExecuteCommand(Command *command);
	void Undo();
	void Redo();
	void SetSelectedNode(NodeDescriptor *selectedNode);
	NodeDescriptor *GetSelectedNode();
	FireCube::Event<NodeDescriptor *> selectedNodeChanged;
	FireCube::Event<> stateChanged;
private:
	NodeDescriptor *selectedNode;
	std::vector<Command *> commands;
	int lastExecutedCommand;
};