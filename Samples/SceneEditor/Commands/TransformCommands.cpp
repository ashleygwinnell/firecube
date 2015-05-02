#include "TransformCommands.h"
#include "../SceneDescriptor.h"

using namespace FireCube;

SetTranslationCommand::SetTranslationCommand(EditorState *editorState, NodeDescriptor *target, vec3 oldTranslation, vec3 newTranslation) : Command(editorState), target(target), oldTranslation(oldTranslation), newTranslation(newTranslation)
{

}

void SetTranslationCommand::Do()
{
	target->GetNode()->SetTranslation(newTranslation);
}

void SetTranslationCommand::Undo()
{
	target->GetNode()->SetTranslation(oldTranslation);
}

SetRotationCommand::SetRotationCommand(EditorState *editorState, NodeDescriptor *target, mat4 oldRotation, mat4 newRotation) : Command(editorState), target(target), oldRotation(oldRotation), newRotation(newRotation)
{

}

void SetRotationCommand::Do()
{
	target->GetNode()->SetRotation(newRotation);
}

void SetRotationCommand::Undo()
{
	target->GetNode()->SetRotation(oldRotation);
}

SetScaleCommand::SetScaleCommand(EditorState *editorState, NodeDescriptor *target, vec3 oldScale, vec3 newScale) : Command(editorState), target(target), oldScale(oldScale), newScale(newScale)
{

}

void SetScaleCommand::Do()
{
	target->GetNode()->SetScale(newScale);
}

void SetScaleCommand::Undo()
{
	target->GetNode()->SetScale(oldScale);
}