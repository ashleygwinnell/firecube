#include "RemoveComponentCommand.h"
#include "../EditorState.h"

using namespace FireCube;

RemoveComponentCommand::RemoveComponentCommand(EditorState *editorState, FireCube::Component *component, std::function<FireCube::Component *(Engine *, Node *)> creationFunction) : Command(editorState), component(component), creationFunction(creationFunction), node(component->GetNode())
{

}

RemoveComponentCommand::~RemoveComponentCommand()
{

}

void RemoveComponentCommand::Do()
{
	node->RemoveComponent(component);
	editorState->selectedNodeChanged(editorState, node);	
}

void RemoveComponentCommand::Undo()
{
	component = creationFunction(editorState->GetEngine(), node);
	editorState->selectedNodeChanged(editorState, node);
}