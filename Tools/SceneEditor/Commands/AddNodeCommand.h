#pragma once

#include "FireCube.h"
#include "Command.h"

class NodeDescriptor;

class AddNodeCommand : public Command
{
public:
	AddNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, NodeDescriptor *parent);
	~AddNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	
	FireCube::SharedPtr<FireCube::Node> node;
	NodeDescriptor *nodeDesc;
	NodeDescriptor *parent;
	bool shouldDelete;
};