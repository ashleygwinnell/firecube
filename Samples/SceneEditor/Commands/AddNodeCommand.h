#pragma once

#include "Command.h"

class NodeDescriptor;

class AddNodeCommand : public Command
{
public:
	AddNodeCommand(EditorState *editorState, NodeDescriptor *node, NodeDescriptor *parent);
	~AddNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	NodeDescriptor *node;
	NodeDescriptor *parent;
};