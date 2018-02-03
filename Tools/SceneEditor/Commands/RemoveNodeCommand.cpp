#include "RemoveNodeCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

RemoveNodeCommand::RemoveNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc) : Command(editorState, description), nodeDesc(nodeDesc), parent(nodeDesc->GetParent()), shouldDelete(false)
{

}

RemoveNodeCommand::~RemoveNodeCommand()
{
	if (shouldDelete)
	{
		delete nodeDesc;
	}
}

void RemoveNodeCommand::Do()
{
	node = nodeDesc->GetNode();
	nodeDesc->Remove();
	shouldDelete = true;
	for (auto &component : nodeDesc->GetComponents())
	{
		editorState->componentRemoved(editorState, component);
	}
	editorState->SetSelectedNode(nullptr);		
}

void RemoveNodeCommand::Undo()
{
	nodeDesc->SetParent(parent);
	shouldDelete = false;
	for (auto &component : nodeDesc->GetComponents())
	{
		editorState->componentAdded(editorState, component);
	}
	editorState->SetSelectedNode(nodeDesc);	
	node.Reset();
}