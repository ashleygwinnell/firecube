#include "RemoveNodeCommand.h"
#include "../EditorState.h"

RemoveNodeCommand::RemoveNodeCommand(EditorState *editorState, FireCube::Node *node) : Command(editorState), node(node), parent(node->GetParent())
{

}

RemoveNodeCommand::~RemoveNodeCommand()
{
	
}

void RemoveNodeCommand::Do()
{
	node->Remove();
	editorState->nodeRemoved(editorState, node);
	editorState->SetSelectedNode(nullptr);		
}

void RemoveNodeCommand::Undo()
{
	node->SetParent(parent);
	editorState->nodeAdded(editorState, node);
	editorState->SetSelectedNode(node);	
}