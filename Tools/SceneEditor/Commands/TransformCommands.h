#pragma once

#include "FireCube.h"
#include "Command.h"

class SetTranslationCommand : public Command
{
public:
	SetTranslationCommand(EditorState *editorState, FireCube::Node *target, FireCube::vec3 oldTranslation, FireCube::vec3 newTranslation);
	virtual void Do();
	virtual void Undo();

private:
	FireCube::Node *target;
	FireCube::vec3 oldTranslation;
	FireCube::vec3 newTranslation;
};

class SetRotationCommand : public Command
{
public:
	SetRotationCommand(EditorState *editorState, FireCube::Node *target, FireCube::mat4 oldRotation, FireCube::mat4 newRotation);
	virtual void Do();
	virtual void Undo();

private:
	FireCube::Node *target;
	FireCube::mat4 oldRotation;
	FireCube::mat4 newRotation;
};

class SetScaleCommand : public Command
{
public:
	SetScaleCommand(EditorState *editorState, FireCube::Node *target, FireCube::vec3 oldScale, FireCube::vec3 newScale);
	virtual void Do();
	virtual void Undo();

private:
	FireCube::Node *target;
	FireCube::vec3 oldScale;
	FireCube::vec3 newScale;
};