#include "GroupCommand.h"
#include "../EditorState.h"

GroupCommand::GroupCommand(EditorState *editorState, const std::string &description, const std::vector<Command *> &commands) : Command(editorState, description), commands(commands)
{
	
}

GroupCommand::~GroupCommand()
{
	for (auto command : commands)
	{
		delete command;
	}
}

void GroupCommand::Do()
{
	for (auto command : commands)
	{
		command->Do();
	}
}

void GroupCommand::Undo()
{
	for (auto i = commands.rbegin(); i != commands.rend(); ++i)
	{
		(*i)->Undo();
	}
}