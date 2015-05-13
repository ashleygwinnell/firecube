#include "TransformCommands.h"
#include "../EditorState.h"

using namespace FireCube;

SetTranslationCommand::SetTranslationCommand(EditorState *editorState, Node *target, vec3 oldTranslation, vec3 newTranslation) : Command(editorState), target(target), oldTranslation(oldTranslation), newTranslation(newTranslation)
{

}

void SetTranslationCommand::Do()
{
	target->SetTranslation(newTranslation);
	editorState->nodeChanged(editorState);
}

void SetTranslationCommand::Undo()
{
	target->SetTranslation(oldTranslation);
	editorState->nodeChanged(editorState);
}

SetRotationCommand::SetRotationCommand(EditorState *editorState, Node *target, mat4 oldRotation, mat4 newRotation) : Command(editorState), target(target), oldRotation(oldRotation), newRotation(newRotation)
{

}

void SetRotationCommand::Do()
{
	target->SetRotation(newRotation);
	editorState->nodeChanged(editorState);
}

void SetRotationCommand::Undo()
{
	target->SetRotation(oldRotation);
	editorState->nodeChanged(editorState);
}

SetScaleCommand::SetScaleCommand(EditorState *editorState, Node *target, vec3 oldScale, vec3 newScale) : Command(editorState), target(target), oldScale(oldScale), newScale(newScale)
{

}

void SetScaleCommand::Do()
{
	target->SetScale(newScale);
	editorState->nodeChanged(editorState);
}

void SetScaleCommand::Undo()
{
	target->SetScale(oldScale);
	editorState->nodeChanged(editorState);
}