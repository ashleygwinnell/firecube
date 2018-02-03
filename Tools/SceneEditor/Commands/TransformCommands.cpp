#include "TransformCommands.h"
#include "../EditorState.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

SetTranslationCommand::SetTranslationCommand(EditorState *editorState, const std::string &description, NodeDescriptor *target, vec3 oldTranslation, vec3 newTranslation) : Command(editorState, description), target(target), oldTranslation(oldTranslation), newTranslation(newTranslation)
{

}

void SetTranslationCommand::Do()
{
	target->SetTranslation(newTranslation);
}

void SetTranslationCommand::Undo()
{
	target->SetTranslation(oldTranslation);
}

SetRotationCommand::SetRotationCommand(EditorState *editorState, const std::string &description, NodeDescriptor *target, vec3 oldRotation, vec3 newRotation) : Command(editorState, description), target(target), oldRotation(oldRotation), newRotation(newRotation)
{

}

void SetRotationCommand::Do()
{
	target->SetRotation(newRotation);
}

void SetRotationCommand::Undo()
{
	target->SetRotation(oldRotation);
}

SetScaleCommand::SetScaleCommand(EditorState *editorState, const std::string &description, NodeDescriptor *target, vec3 oldScale, vec3 newScale) : Command(editorState, description), target(target), oldScale(oldScale), newScale(newScale)
{

}

void SetScaleCommand::Do()
{
	target->SetScale(newScale);
}

void SetScaleCommand::Undo()
{
	target->SetScale(oldScale);
}