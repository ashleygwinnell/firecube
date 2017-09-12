#pragma once

#include "FireCube.h"
#include <vector>

class Command;
class NodeDescriptor;
class ComponentDescriptor;

class EditorState : public FireCube::Object
{
	FIRECUBE_OBJECT(EditorState)
public:
	EditorState(FireCube::Engine *engine);
	void ExecuteCommand(Command *command, Command *commandToReplace = nullptr);
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
	FireCube::Event<> newSceneCreated;
	FireCube::Event<ComponentDescriptor *> componentRemoved;
	FireCube::Event<ComponentDescriptor *> componentAdded;
	FireCube::Event<Command *> commandExecuted;
	FireCube::Event<Command *> undoPerformed;
	FireCube::Event<Command *> redoPerformed;
	FireCube::Event<> startMaterialPick;
	FireCube::Event<FireCube::Material *> materialPicked;
	FireCube::Event<> showMaterialEditor;
	FireCube::Event<const std::string &> addMesh;
	FireCube::Event<const std::string &> showScriptEditor;
	FireCube::Event<const std::string &> addPrefab;
	FireCube::Event<> switchedToTranslateGizmo;
	FireCube::Event<> switchedToRotateGizmo;
	FireCube::Event<> switchedToScaleGizmo;

private:
	
	NodeDescriptor *selectedNode;
	std::vector<Command *> commands;
	int lastExecutedCommand;
	std::string currentSceneFile;
	std::map<FireCube::Node *, NodeDescriptor *> nodeMap;
};