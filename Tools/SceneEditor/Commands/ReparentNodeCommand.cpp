#include "ReparentNodeCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

ReparentNodeCommand::ReparentNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, NodeDescriptor *newParent) : Command(editorState, description), nodeDesc(nodeDesc), newParent(newParent), oldParent(nodeDesc->GetParent())
{

}

ReparentNodeCommand::~ReparentNodeCommand()
{

}

void ReparentNodeCommand::Do()
{
	nodeDesc->SetParent(newParent);
}

void ReparentNodeCommand::Undo()
{
	nodeDesc->SetParent(oldParent);
}