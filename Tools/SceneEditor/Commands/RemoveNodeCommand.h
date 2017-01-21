#pragma once

#include "FireCube.h"
#include "Command.h"

class NodeDescriptor;

class RemoveNodeCommand : public Command
{
public:
	RemoveNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc);
	~RemoveNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	NodeDescriptor *nodeDesc;
	NodeDescriptor *parent;
	FireCube::SharedPtr<FireCube::Node> node;
	bool shouldDelete;
};