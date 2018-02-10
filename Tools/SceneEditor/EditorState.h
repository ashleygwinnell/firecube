#pragma once

#include "FireCube.h"
#include <vector>

class Command;
class NodeDescriptor;
class ComponentDescriptor;

enum class RenderingLoopState
{
	RUNNING, PAUSED, STOPPED
};

class Settings
{
public:
	std::vector<std::string> recentSceneFiles;
	std::string externalCodeEditorPath;
};

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
	void SetRenderingLoopState(RenderingLoopState state);
	RenderingLoopState GetRenderingLoopState() const;
	Settings &GetSettings();

	FireCube::Event<NodeDescriptor *> selectedNodeChanged;
	FireCube::Event<ComponentDescriptor *> componentRemoved;
	FireCube::Event<ComponentDescriptor *> componentAdded;
	FireCube::Event<> startMaterialPick;
	FireCube::Event<FireCube::Material *> materialPicked;
	FireCube::Event<> showMaterialEditor;
	FireCube::Event<const std::string &> addMesh;
	FireCube::Event<const std::string &> showScriptEditor;
	FireCube::Event<const std::string &> addPrefab;
	FireCube::Event<> switchedToTranslateGizmo;
	FireCube::Event<> switchedToRotateGizmo;
	FireCube::Event<> switchedToScaleGizmo;
	FireCube::Event<> startRenderLoop;
	FireCube::Event<> pauseRenderLoop;
	FireCube::Event<> stopRenderLoop;
	FireCube::Event<> resetParticleEmitters;

private:
	Settings settings;
	NodeDescriptor *selectedNode;
	std::vector<Command *> commands;
	int lastExecutedCommand;
	std::string currentSceneFile;
	std::map<FireCube::Node *, NodeDescriptor *> nodeMap;
	RenderingLoopState renderingLoopState;
};