#pragma once

#include "FireCube.h"
#include "Command.h"

class NodeDescriptor;

class ReparentNodeCommand : public Command
{
public:
	ReparentNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, NodeDescriptor *newParent);
	~ReparentNodeCommand();
	virtual void Do();
	virtual void Undo();
private:
	NodeDescriptor *nodeDesc;
	NodeDescriptor *newParent;
	NodeDescriptor *oldParent;
	FireCube::vec3 oldTranslation;
};