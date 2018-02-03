#include "RemoveComponentCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../Descriptors/ComponentDescriptor.h"

using namespace FireCube;

RemoveComponentCommand::RemoveComponentCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, ComponentDescriptor *componentDesc, FireCube::Engine *engine) : Command(editorState, description), nodeDesc(nodeDesc), componentDesc(componentDesc), engine(engine), shouldDelete(false)
{

}

RemoveComponentCommand::~RemoveComponentCommand()
{
	if (shouldDelete)
	{
		delete componentDesc;
	}
}

void RemoveComponentCommand::Do()
{
	editorState->componentRemoved(editorState, componentDesc);
	nodeDesc->RemoveComponent(componentDesc);
	if (componentDesc->GetComponent())
	{
		nodeDesc->GetNode()->RemoveComponent(componentDesc->GetComponent());
	}
	componentDesc->SetParent(nullptr);
	shouldDelete = true;
}

void RemoveComponentCommand::Undo()
{
	nodeDesc->AddComponent(componentDesc);
	componentDesc->CreateComponent(nodeDesc->GetNode(), engine);
	componentDesc->SetParent(nodeDesc);
	shouldDelete = false;
	editorState->componentAdded(editorState, componentDesc);
}