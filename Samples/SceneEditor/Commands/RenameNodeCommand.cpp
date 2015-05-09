#include "RenameNodeCommand.h"
#include "../EditorState.h"

RenameNodeCommand::RenameNodeCommand(EditorState *editorState, FireCube::Node *node, const std::string &newName) : Command(editorState), node(node), newName(newName), oldName(node->GetName())
{

}

RenameNodeCommand::~RenameNodeCommand()
{

}

void RenameNodeCommand::Do()
{
	node->SetName(newName);
	editorState->nodeRenamed(editorState, node);	
}

void RenameNodeCommand::Undo()
{
	node->SetName(oldName);
	editorState->nodeRenamed(editorState, node);
}