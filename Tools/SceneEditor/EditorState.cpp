#include "EditorState.h"
#include "Commands/Command.h"

using namespace FireCube;

EditorState::EditorState(Engine *engine) : Object(engine), lastExecutedCommand(-1), selectedNode(nullptr)
{

}

void EditorState::ExecuteCommand(Command *command)
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
	commands.push_back(command);	
	lastExecutedCommand = commands.size() - 1;

	commandExecuted(this, command);
}

void EditorState::Undo()
{
	if (lastExecutedCommand != -1)
	{
		commands[lastExecutedCommand]->Undo();
		lastExecutedCommand--;
		undoPerformed(this, commands[lastExecutedCommand + 1]);
	}

	stateChanged(this);
}

void EditorState::Redo()
{
	if (lastExecutedCommand < ((int) commands.size()) - 1 && commands.empty() == false)
	{
		lastExecutedCommand++;
		commands[lastExecutedCommand]->Do();		
		redoPerformed(this, commands[lastExecutedCommand]);
	}

	stateChanged(this);
}

void EditorState::SetSelectedNode(FireCube::Node *selectedNode)
{
	this->selectedNode = selectedNode;
	selectedNodeChanged(this, selectedNode);
}

FireCube::Node *EditorState::GetSelectedNode()
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