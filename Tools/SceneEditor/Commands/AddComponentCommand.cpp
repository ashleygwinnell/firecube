#include "AddComponentCommand.h"
#include "../EditorState.h"

using namespace FireCube;

AddComponentCommand::AddComponentCommand(EditorState *editorState, FireCube::Node *node, std::function<FireCube::Component *(Engine *, Node *)> creationFunction) : Command(editorState), node(node), creationFunction(creationFunction)
{

}

AddComponentCommand::~AddComponentCommand()
{
	
}

void AddComponentCommand::Do()
{
	component = creationFunction(editorState->GetEngine(), node);
}

void AddComponentCommand::Undo()
{
	node->RemoveComponent(component);	
}