#include "CustomCommand.h"
#include "../EditorState.h"

using namespace FireCube;

CustomCommand::CustomCommand(EditorState *editorState, std::function<void()> doFunction, std::function<void()> undoFunction) : Command(editorState), doFunction(doFunction), undoFunction(undoFunction)
{

}

CustomCommand::~CustomCommand()
{

}

void CustomCommand::Do()
{
	doFunction();
}

void CustomCommand::Undo()
{
	undoFunction();
}