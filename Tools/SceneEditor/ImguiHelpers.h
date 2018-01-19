#pragma once

#include "FireCube.h"

class EditorState;
class Command;

class Vec3InputHelper
{
public:
	Vec3InputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec3()> getValue, std::function<Command *(FireCube::vec3, FireCube::vec3)> setValue);	
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec3()> getValue, std::function<void(FireCube::vec3)> setValue);
private:
	FireCube::vec3 prevValue;
	bool isActive;
	Command *prevCommand;
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

class FloatInputHelper
{
public:
	FloatInputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<float()> getValue, std::function<Command *(float, float)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<float()> getValue, std::function<void(float)> setValue);
private:
	float prevValue;
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

class ColorInputHelper
{
public:
	ColorInputHelper();
	void Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec3()> getValue, std::function<Command *(FireCube::vec3, FireCube::vec3)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec3()> getValue, std::function<void(FireCube::vec3)> setValue);
private:
	FireCube::vec3 prevValue;
	bool isActive;
	Command *prevCommand;

};

class TextInputHelper
{
public:
	TextInputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<std::string()> getValue, std::function<Command *(const std::string &, const std::string &)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<std::string()> getValue, std::function<void(const std::string &)> setValue);
private:
	std::string prevValue;
	bool isActive;
	Command *prevCommand;
	char val[256];
};