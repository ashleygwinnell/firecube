#pragma once

#include "FireCube.h"
#include "Command.h"

class NodeDescriptor;

class SetTranslationCommand : public Command
{
public:
	SetTranslationCommand(EditorState *editorState, NodeDescriptor *target, FireCube::vec3 oldTranslation, FireCube::vec3 newTranslation);
	virtual void Do();
	virtual void Undo();

private:
	NodeDescriptor *target;
	FireCube::vec3 oldTranslation;
	FireCube::vec3 newTranslation;
};

class SetRotationCommand : public Command
{
public:
	SetRotationCommand(EditorState *editorState, NodeDescriptor *target, FireCube::mat4 oldRotation, FireCube::mat4 newRotation);
	virtual void Do();
	virtual void Undo();

private:
	NodeDescriptor *target;
	FireCube::mat4 oldRotation;
	FireCube::mat4 newRotation;
};

class SetScaleCommand : public Command
{
public:
	SetScaleCommand(EditorState *editorState, NodeDescriptor *target, FireCube::vec3 oldScale, FireCube::vec3 newScale);
	virtual void Do();
	virtual void Undo();

private:
	NodeDescriptor *target;
	FireCube::vec3 oldScale;
	FireCube::vec3 newScale;
};