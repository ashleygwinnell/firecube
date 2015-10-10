#pragma once

#include <vector>
#include <functional>
#include "Command.h"
#include "FireCube.h"

class NodeDescriptor;
class ComponentDescriptor;

class AddComponentCommand : public Command
{
public:
	AddComponentCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, ComponentDescriptor *componentDesc, FireCube::Engine *engine);
	~AddComponentCommand();
	virtual void Do();
	virtual void Undo();
private:
	NodeDescriptor *nodeDesc;
	ComponentDescriptor *componentDesc;
	FireCube::Engine *engine;
	bool shouldDelete;
};