#pragma once

#include "Command.h"

class NodeDescriptor;

class RemoveNodeCommand : public Command
{
public:
	RemoveNodeCommand(EditorState *editorState, NodeDescriptor *node);
	~RemoveNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	NodeDescriptor *node;
	NodeDescriptor *parent;
};