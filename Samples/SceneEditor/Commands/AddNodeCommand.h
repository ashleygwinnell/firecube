#pragma once

#include "FireCube.h"
#include "Command.h"

class AddNodeCommand : public Command
{
public:
	AddNodeCommand(EditorState *editorState, FireCube::Node *node, FireCube::Node *parent);
	~AddNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	FireCube::SharedPtr<FireCube::Node> node;
	FireCube::Node *parent;
};