#include "AddNodeCommand.h"
#include "../SceneDescriptor.h"
#include "../EditorState.h"

AddNodeCommand::AddNodeCommand(EditorState *editorState, NodeDescriptor *node, NodeDescriptor *parent) : Command(editorState), node(node), parent(parent)
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