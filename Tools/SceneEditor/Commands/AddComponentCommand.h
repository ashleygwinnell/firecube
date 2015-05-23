#pragma once

#include <vector>
#include <functional>
#include "Command.h"
#include "FireCube.h"

class AddComponentCommand : public Command
{
public:
	AddComponentCommand(EditorState *editorState, const std::string &description, FireCube::Node *node, std::function<FireCube::Component *(FireCube::Engine *, FireCube::Node *)> creationFunction);
	~AddComponentCommand();
	virtual void Do();
	virtual void Undo();
private:
	FireCube::Node *node;	
	FireCube::Component *component;
	std::function<FireCube::Component *(FireCube::Engine *, FireCube::Node *)> creationFunction;
};