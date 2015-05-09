#include "AddNodeCommand.h"
#include "../EditorState.h"

AddNodeCommand::AddNodeCommand(EditorState *editorState, FireCube::Node *node, FireCube::Node *parent) : Command(editorState), node(node), parent(parent)
{

}

AddNodeCommand::~AddNodeCommand()
{
	
}

void AddNodeCommand::Do()
{
	node->SetParent(parent);
	editorState->nodeAdded(editorState, node);
	editorState->SetSelectedNode(node);
	
}

void AddNodeCommand::Undo()
{
	node->Remove();
	editorState->nodeRemoved(editorState, node);
	editorState->SetSelectedNode(nullptr);	
}