#include "RenameNodeCommand.h"
#include "../EditorState.h"
#include "../NodeDescriptor.h"

RenameNodeCommand::RenameNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, const std::string &newName) : Command(editorState, description), nodeDesc(nodeDesc), newName(newName), oldName(nodeDesc->GetNode()->GetName())
{

}

RenameNodeCommand::~RenameNodeCommand()
{

}

void RenameNodeCommand::Do()
{
	nodeDesc->SetName(newName);
	editorState->nodeRenamed(editorState, nodeDesc);
}

void RenameNodeCommand::Undo()
{
	nodeDesc->SetName(oldName);
	editorState->nodeRenamed(editorState, nodeDesc);
}