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
}

void EditorState::Undo()
{
	if (lastExecutedCommand != -1)
	{
		commands[lastExecutedCommand]->Undo();
		lastExecutedCommand--;
	}

	stateChanged(this);
}

void EditorState::Redo()
{
	if (lastExecutedCommand < ((int) commands.size()) - 1 && commands.empty() == false)
	{
		lastExecutedCommand++;
		commands[lastExecutedCommand]->Do();		
	}

	stateChanged(this);
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