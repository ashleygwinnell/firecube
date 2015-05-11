#pragma once

#include <vector>
#include <functional>
#include "Command.h"
#include "FireCube.h"

class RemoveComponentCommand : public Command
{
public:
	RemoveComponentCommand(EditorState *editorState, FireCube::Component *component, std::function<FireCube::Component *(FireCube::Engine *, FireCube::Node *)> creationFunction);
	~RemoveComponentCommand();
	virtual void Do();
	virtual void Undo();
private:		
	FireCube::Node *node;
	FireCube::Component *component;
	std::function<FireCube::Component *(FireCube::Engine *, FireCube::Node *)> creationFunction;
};