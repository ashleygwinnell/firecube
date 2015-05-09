#pragma once

#include "FireCube.h"
#include "Command.h"

class RenameNodeCommand : public Command
{
public:
	RenameNodeCommand(EditorState *editorState, FireCube::Node *node, const std::string &newName);
	~RenameNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	FireCube::SharedPtr<FireCube::Node> node;
	std::string newName;
	std::string oldName;
};