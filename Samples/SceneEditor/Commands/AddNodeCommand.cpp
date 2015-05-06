#include "AddNodeCommand.h"
#include "../EditorState.h"

AddNodeCommand::AddNodeCommand(EditorState *editorState, FireCube::Node *node, FireCube::Node *parent) : Command(editorState), node(node), parent(parent)
{

}

AddNodeCommand::~AddNodeCommand()
{
	delete node;
}

void AddNodeCommand::Do()
{
	node->SetParent(parent);
	editorState->SetSelectedNode(node);
}

void AddNodeCommand::Undo()
{
	node->Remove();
	editorState->SetSelectedNode(nullptr);
}