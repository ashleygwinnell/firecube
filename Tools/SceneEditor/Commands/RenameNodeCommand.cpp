#include "RenameNodeCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

RenameNodeCommand::RenameNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, const std::string &newName) : Command(editorState, description), nodeDesc(nodeDesc), newName(newName), oldName(nodeDesc->GetNode()->GetName())
{

}

RenameNodeCommand::~RenameNodeCommand()
{

}

void RenameNodeCommand::Do()
{
	nodeDesc->SetName(newName);
}

void RenameNodeCommand::Undo()
{
	nodeDesc->SetName(oldName);
}