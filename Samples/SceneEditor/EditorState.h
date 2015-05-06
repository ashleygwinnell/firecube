#pragma once

#include "FireCube.h"
#include <vector>

class Command;

class EditorState : public FireCube::Object
{
	OBJECT(EditorState)
public:
	EditorState(FireCube::Engine *engine);
	void ExecuteCommand(Command *command);
	void Undo();
	void Redo();
	void SetSelectedNode(FireCube::Node *selectedNode);
	FireCube::Node *GetSelectedNode();
	FireCube::Engine *GetEngine();
	FireCube::Event<FireCube::Node *> selectedNodeChanged;
	FireCube::Event<> stateChanged;	
private:
	
	FireCube::Node *selectedNode;
	std::vector<Command *> commands;
	int lastExecutedCommand;
};