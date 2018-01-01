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

class HexInputHelper
{
public:
	HexInputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<unsigned int()> getValue, std::function<Command *(unsigned int, unsigned int)> setValue);	
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<unsigned int()> getValue, std::function<void(unsigned int)> setValue);
private:	
	unsigned int prevValue;
	bool isActive;
	Command *prevCommand;
	char val[9];
};

class CheckBoxHelper
{
public:
	CheckBoxHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<bool()> getValue, std::function<Command *(bool, bool)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<bool()> getValue, std::function<void(bool)> setValue);
private:
	bool prevValue;
	bool isActive;
	Command *prevCommand;
};