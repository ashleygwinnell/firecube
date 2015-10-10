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
	nodeDesc->Remove();
	shouldDelete = true;
	editorState->nodeRemoved(editorState, nodeDesc);
	editorState->SetSelectedNode(nullptr);		
}

void RemoveNodeCommand::Undo()
{
	nodeDesc->SetParent(parent);
	shouldDelete = false;
	editorState->nodeAdded(editorState, nodeDesc);
	editorState->SetSelectedNode(nodeDesc);	
}