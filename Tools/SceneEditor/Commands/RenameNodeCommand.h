#pragma once

#include "FireCube.h"
#include "Command.h"

class NodeDescriptor;

class RenameNodeCommand : public Command
{
public:
	RenameNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, const std::string &newName);
	~RenameNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	NodeDescriptor *nodeDesc;
	std::string newName;
	std::string oldName;
};