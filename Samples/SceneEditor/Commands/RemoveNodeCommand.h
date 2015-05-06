#pragma once

#include "FireCube.h"
#include "Command.h"

class RemoveNodeCommand : public Command
{
public:
	RemoveNodeCommand(EditorState *editorState, FireCube::Node *node);
	~RemoveNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	FireCube::Node *node;
	FireCube::Node *parent;
};