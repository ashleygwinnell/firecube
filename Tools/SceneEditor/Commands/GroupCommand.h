#pragma once

#include <vector>
#include "Command.h"

class GroupCommand : public Command
{
public:
	GroupCommand(EditorState *editorState, const std::string &description, const std::vector<Command *> &commands);
	~GroupCommand();
	virtual void Do();
	virtual void Undo();
private:	
	std::vector<Command *> commands;
};