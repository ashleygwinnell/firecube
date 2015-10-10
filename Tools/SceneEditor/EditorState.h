#pragma once

#include "FireCube.h"
#include <vector>

class Command;
class NodeDescriptor;
class ComponentDescriptor;

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
	FireCube::Engine *GetEngine();
	void SetCurrentSceneFile(const std::string &currentSceneFile);
	std::string GetCurrentSceneFile() const;
	void ClearCommands();
	bool HasUndo() const;
	bool HasRedo() const;
	Command *GetCurrentUndoCommand() const;
	Command *GetCurrentRedoCommand() const;
	std::map<FireCube::Node *, NodeDescriptor *> &GetNodeMap();

	FireCube::Event<NodeDescriptor *> selectedNodeChanged;
	FireCube::Event<NodeDescriptor *> nodeAdded;
	FireCube::Event<NodeDescriptor *> nodeRemoved;
	FireCube::Event<NodeDescriptor *> nodeRenamed;
	FireCube::Event<> stateChanged;	
	FireCube::Event<> sceneChanged;
	FireCube::Event<> nodeChanged;
	FireCube::Event<ComponentDescriptor *> componentRemoved;
	FireCube::Event<ComponentDescriptor *> componentAdded;
	FireCube::Event<Command *> commandExecuted;
	FireCube::Event<Command *> undoPerformed;
	FireCube::Event<Command *> redoPerformed;
private:
	
	NodeDescriptor *selectedNode;
	std::vector<Command *> commands;
	int lastExecutedCommand;
	std::string currentSceneFile;
	std::map<FireCube::Node *, NodeDescriptor *> nodeMap;
};