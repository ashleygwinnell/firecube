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
	void SetCurrentSceneFile(const std::string &currentSceneFile);
	std::string GetCurrentSceneFile() const;

	FireCube::Event<FireCube::Node *> selectedNodeChanged;
	FireCube::Event<FireCube::Node *> nodeAdded;
	FireCube::Event<FireCube::Node *> nodeRemoved;
	FireCube::Event<> stateChanged;	
private:
	
	FireCube::Node *selectedNode;
	std::vector<Command *> commands;
	int lastExecutedCommand;
	std::string currentSceneFile;
};