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
	editorState->SetSelectedNode(nodeDesc);
}

void AddNodeCommand::Undo()
{
	nodeDesc->Remove();
	shouldDelete = true;
	editorState->SetSelectedNode(nullptr);	
}
