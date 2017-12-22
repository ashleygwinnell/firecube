#pragma once

#include "FireCube.h"

class EditorState;
class Command;

class Vec3InputHelper
{
public:
	Vec3InputHelper();

	void Render();
	void Init(const std::string &label, EditorState *editorState, std::function<FireCube::vec3()> getValue, std::function<Command *(FireCube::vec3, FireCube::vec3)> setValue);
private:
	std::function<FireCube::vec3()> getValue;
	std::function<Command *(FireCube::vec3, FireCube::vec3)> setValue;
	EditorState *editorState;
	FireCube::vec3 prevValue;
	bool isActive;
	Command *prevCommand;
	std::string label;
};