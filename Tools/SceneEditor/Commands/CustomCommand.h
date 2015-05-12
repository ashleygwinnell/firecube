#pragma once

#include <vector>
#include <functional>
#include "Command.h"
#include "FireCube.h"

class CustomCommand : public Command
{
public:
	CustomCommand(EditorState *editorState, std::function<void ()> doFunction, std::function<void ()> undoFunction);
	~CustomCommand();
	virtual void Do();
	virtual void Undo();
private:	
	std::function<void()> doFunction, undoFunction;
};