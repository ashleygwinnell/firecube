#pragma once

#include "FireCube.h"
#include "Command.h"

class NodeDescriptor;

class SetTranslationCommand : public Command
{
public:
	SetTranslationCommand(EditorState *editorState, const std::string &description, NodeDescriptor *target, FireCube::vec3 oldTranslation, FireCube::vec3 newTranslation);
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
	SetRotationCommand(EditorState *editorState, const std::string &description, NodeDescriptor *target, FireCube::vec3 oldRotation, FireCube::vec3 newRotation);
	virtual void Do();
	virtual void Undo();

private:
	NodeDescriptor *target;
	FireCube::vec3 oldRotation;
	FireCube::vec3 newRotation;
};

class SetScaleCommand : public Command
{
public:
	SetScaleCommand(EditorState *editorState, const std::string &description, NodeDescriptor *target, FireCube::vec3 oldScale, FireCube::vec3 newScale);
	virtual void Do();
	virtual void Undo();

private:
	NodeDescriptor *target;
	FireCube::vec3 oldScale;
	FireCube::vec3 newScale;
};