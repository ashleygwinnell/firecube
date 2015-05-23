#include "AddComponentCommand.h"
#include "../EditorState.h"

using namespace FireCube;

AddComponentCommand::AddComponentCommand(EditorState *editorState, const std::string &description, FireCube::Node *node, std::function<FireCube::Component *(Engine *, Node *)> creationFunction) : Command(editorState, description), node(node), creationFunction(creationFunction)
{

}

AddComponentCommand::~AddComponentCommand()
{
	
}

void AddComponentCommand::Do()
{
	component = creationFunction(editorState->GetEngine(), node);
	editorState->componentAdded(editorState, component);
	editorState->sceneChanged(editorState);
}

void AddComponentCommand::Undo()
{
	editorState->componentRemoved(editorState, component);
	node->RemoveComponent(component);	
	editorState->sceneChanged(editorState);
}