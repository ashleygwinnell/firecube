#include "AddNodeCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

AddNodeCommand::AddNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, NodeDescriptor *parent) : Command(editorState, description), nodeDesc(nodeDesc), parent(parent)
{
	nodeDesc->Instantiate(nullptr, editorState->GetEngine(), editorState->GetNodeMap());
	node = nodeDesc->GetNode();
}

AddNodeCommand::~AddNodeCommand()
{
	if (!nodeDesc->GetParent())
	{
		delete nodeDesc;
	}
}

void AddNodeCommand::Do()
{
	nodeDesc->SetParent(parent);
	editorState->SetSelectedNode(nodeDesc);
}

void AddNodeCommand::Undo()
{
	nodeDesc->Remove();
	editorState->SetSelectedNode(nullptr);
}
