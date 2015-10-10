#pragma once

#include <vector>
#include <functional>
#include "Command.h"
#include "FireCube.h"

class NodeDescriptor;
class ComponentDescriptor;

class RemoveComponentCommand : public Command
{
public:
	RemoveComponentCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, ComponentDescriptor *componentDesc, FireCube::Engine *engine);
	~RemoveComponentCommand();
	virtual void Do();
	virtual void Undo();
private:		
	NodeDescriptor *nodeDesc;
	ComponentDescriptor *componentDesc;
	FireCube::Engine *engine;
	bool shouldDelete;
};