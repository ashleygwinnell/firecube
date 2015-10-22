#include "AddNodeCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

AddNodeCommand::AddNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, NodeDescriptor *parent) : Command(editorState, description), nodeDesc(nodeDesc), parent(parent), shouldDelete(true)
{
	nodeDesc->Instantiate(nullptr, editorState->GetEngine(), editorState->GetNodeMap());
	node = nodeDesc->GetNode();
}

AddNodeCommand::~AddNodeCommand()
{
	if (shouldDelete)
	{		
		delete nodeDesc;		
	}
}

void AddNodeCommand::Do()
{
	nodeDesc->SetParent(parent);
	shouldDelete = false;	
	FireNodeAddedEvent(nodeDesc);
	editorState->SetSelectedNode(nodeDesc);
}

void AddNodeCommand::Undo()
{
	nodeDesc->Remove();
	shouldDelete = true;	
	FireNodeRemovedEvent(nodeDesc);
	editorState->SetSelectedNode(nullptr);	
}

void AddNodeCommand::FireNodeAddedEvent(NodeDescriptor *nodeDesc)
{
	editorState->nodeAdded(editorState, nodeDesc);

	for (auto child : nodeDesc->GetChildren())
	{
		FireNodeAddedEvent(child);
	}
}

void AddNodeCommand::FireNodeRemovedEvent(NodeDescriptor *nodeDesc)
{
	editorState->nodeRemoved(editorState, nodeDesc);

	for (auto child : nodeDesc->GetChildren())
	{
		FireNodeRemovedEvent(child);
	}
}