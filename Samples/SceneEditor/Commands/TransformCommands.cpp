#include "TransformCommands.h"

using namespace FireCube;

SetTranslationCommand::SetTranslationCommand(EditorState *editorState, Node *target, vec3 oldTranslation, vec3 newTranslation) : Command(editorState), target(target), oldTranslation(oldTranslation), newTranslation(newTranslation)
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

SetRotationCommand::SetRotationCommand(EditorState *editorState, Node *target, mat4 oldRotation, mat4 newRotation) : Command(editorState), target(target), oldRotation(oldRotation), newRotation(newRotation)
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

SetScaleCommand::SetScaleCommand(EditorState *editorState, Node *target, vec3 oldScale, vec3 newScale) : Command(editorState), target(target), oldScale(oldScale), newScale(newScale)
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