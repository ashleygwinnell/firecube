#include "RemoveNodeCommand.h"
#include "../SceneDescriptor.h"
#include "../EditorState.h"

RemoveNodeCommand::RemoveNodeCommand(EditorState *editorState, NodeDescriptor *node) : Command(editorState), node(node), parent(node->GetParent())
{

}

RemoveNodeCommand::~RemoveNodeCommand()
{
	
}

void RemoveNodeCommand::Do()
{
	node->Remove();
	editorState->SetSelectedNode(nullptr);	
}

void RemoveNodeCommand::Undo()
{
	node->SetParent(parent);
	editorState->SetSelectedNode(node);
}