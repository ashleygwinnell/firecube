#include "EditorState.h"
#include "Commands/Command.h"

using namespace FireCube;

EditorState::EditorState(Engine *engine) : Object(engine), lastExecutedCommand(-1), selectedNode(nullptr), renderingLoopState(RenderingLoopState::STOPPED)
{

}

void EditorState::ExecuteCommand(Command *command, Command *commandToReplace)
{	
	for (int i = lastExecutedCommand + 1; i < (int)commands.size(); ++i)
	{
		delete commands[i];
	}

	if (commands.empty() == false)
	{
		commands.erase(commands.begin() + (int) (lastExecutedCommand + 1), commands.end());
	}
	
	command->Do();
	
	if (lastExecutedCommand != -1 && commands[lastExecutedCommand] == commandToReplace)
	{
		delete commands[lastExecutedCommand];
		commands[lastExecutedCommand] = command;
	}
	else
	{
		commands.push_back(command);
		lastExecutedCommand = commands.size() - 1;
	}
}

void EditorState::Undo()
{
	if (lastExecutedCommand != -1)
	{
		commands[lastExecutedCommand]->Undo();
		lastExecutedCommand--;
	}
}

void EditorState::Redo()
{
	if (lastExecutedCommand < ((int) commands.size()) - 1 && commands.empty() == false)
	{
		lastExecutedCommand++;
		commands[lastExecutedCommand]->Do();
	}
}

void EditorState::SetSelectedNode(NodeDescriptor *selectedNode)
{
	this->selectedNode = selectedNode;
	selectedNodeChanged(this, selectedNode);
}

NodeDescriptor *EditorState::GetSelectedNode()
{
	return selectedNode;
}

FireCube::Engine *EditorState::GetEngine()
{
	return engine;
}

void EditorState::SetCurrentSceneFile(const std::string &currentSceneFile)
{
	this->currentSceneFile = currentSceneFile;
}

std::string EditorState::GetCurrentSceneFile() const
{
	return currentSceneFile;
}

void EditorState::ClearCommands()
{
	commands.clear();
	lastExecutedCommand = -1;
}

bool EditorState::HasUndo() const
{
	return lastExecutedCommand != -1;	
}
bool EditorState::HasRedo() const
{
	return lastExecutedCommand < ((int)commands.size()) - 1 && commands.empty() == false;
}

Command *EditorState::GetCurrentUndoCommand() const
{
	if (lastExecutedCommand != -1)
	{
		return commands[lastExecutedCommand];		
	}
	else
	{
		return nullptr;
	}
}

Command *EditorState::GetCurrentRedoCommand() const
{
	if (lastExecutedCommand < ((int)commands.size()) - 1 && commands.empty() == false)
	{		
		return commands[lastExecutedCommand + 1];
	}
	else
	{
		return nullptr;
	}
}

std::map<FireCube::Node *, NodeDescriptor *> &EditorState::GetNodeMap()
{
	return nodeMap;
}

RenderingLoopState EditorState::GetRenderingLoopState() const
{
	return renderingLoopState;
}

Settings &EditorState::GetSettings()
{
	return settings;
}

void EditorState::SetRenderingLoopState(RenderingLoopState state)
{
	renderingLoopState = state;
}
