#include "RemoveComponentCommand.h"
#include "../EditorState.h"

using namespace FireCube;

RemoveComponentCommand::RemoveComponentCommand(EditorState *editorState, const std::string &description, FireCube::Component *component, std::function<FireCube::Component *(Engine *, Node *)> creationFunction) : Command(editorState, description), component(component), creationFunction(creationFunction), node(component->GetNode())
{

}

RemoveComponentCommand::~RemoveComponentCommand()
{

}

void RemoveComponentCommand::Do()
{
	editorState->componentRemoved(editorState, component);
	node->RemoveComponent(component);	
	editorState->sceneChanged(editorState);
}

void RemoveComponentCommand::Undo()
{
	component = creationFunction(editorState->GetEngine(), node);
	editorState->componentAdded(editorState, component);
	editorState->sceneChanged(editorState);
}