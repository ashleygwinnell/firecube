#include "AddComponentCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../Descriptors/ComponentDescriptor.h"
using namespace FireCube;

AddComponentCommand::AddComponentCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, ComponentDescriptor *componentDesc, Engine *engine) : Command(editorState, description), nodeDesc(nodeDesc), componentDesc(componentDesc), engine(engine), shouldDelete(true)
{

}

AddComponentCommand::~AddComponentCommand()
{
	if (shouldDelete)
	{
		delete componentDesc;
	}
}

void AddComponentCommand::Do()
{	
	nodeDesc->AddComponent(componentDesc);
	componentDesc->CreateComponent(nodeDesc->GetNode(), engine);
	componentDesc->SetParent(nodeDesc);
	shouldDelete = false;
	editorState->componentAdded(editorState, componentDesc);
	editorState->sceneChanged(editorState);
}

void AddComponentCommand::Undo()
{
	editorState->componentRemoved(editorState, componentDesc);
	nodeDesc->RemoveComponent(componentDesc);
	nodeDesc->GetNode()->RemoveComponent(componentDesc->GetComponent());
	componentDesc->SetParent(nullptr);
	shouldDelete = true;
	editorState->sceneChanged(editorState);
}