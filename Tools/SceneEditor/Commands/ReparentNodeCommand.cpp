#include "ReparentNodeCommand.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

ReparentNodeCommand::ReparentNodeCommand(EditorState *editorState, const std::string &description, NodeDescriptor *nodeDesc, NodeDescriptor *newParent) : Command(editorState, description),
	nodeDesc(nodeDesc), newParent(newParent), oldParent(nodeDesc->GetParent())
{

}

ReparentNodeCommand::~ReparentNodeCommand()
{

}

void ReparentNodeCommand::Do()
{
	oldTranslation = nodeDesc->GetTranslation();
	vec3 worldPosition = nodeDesc->GetNode()->GetWorldPosition();
	nodeDesc->SetParent(newParent);
	mat4 mat = newParent->GetNode()->GetWorldTransformation();
	mat.Inverse();
	nodeDesc->SetTranslation(mat * worldPosition);
}

void ReparentNodeCommand::Undo()
{
	nodeDesc->SetParent(oldParent);
	nodeDesc->SetTranslation(oldTranslation);
}