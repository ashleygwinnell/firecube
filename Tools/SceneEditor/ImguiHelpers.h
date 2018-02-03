#pragma once

#include "FireCube.h"
#include "imgui.h"

class EditorState;
class Command;

class Vec2InputHelper
{
public:
	Vec2InputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec2()> getValue, std::function<Command *(FireCube::vec2, FireCube::vec2)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec2()> getValue, std::function<void(FireCube::vec2)> setValue);
private:
	FireCube::vec2 prevValue;
	bool isActive;
	Command *prevCommand;
};

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

class Vec4InputHelper
{
public:
	Vec4InputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec4()> getValue, std::function<Command *(FireCube::vec4, FireCube::vec4)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec4()> getValue, std::function<void(FireCube::vec4)> setValue);
private:
	FireCube::vec4 prevValue;
	bool isActive;
	Command *prevCommand;
};

class UIntInputHelper
{
public:
	UIntInputHelper();

	void Render(const std::string &label, EditorState *editorState, std::function<unsigned int()> getValue, std::function<Command *(unsigned int, unsigned int)> setValue);
	void Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<unsigned int()> getValue, std::function<void(unsigned int)> setValue);
private:
	unsigned int prevValue;
	bool isActive;
	Command *prevCommand;
	char val[9];
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

namespace ImGui
{
	IMGUI_API bool BeginToolbar(const char* str_id, ImVec2 screen_pos, ImVec2 size);
	IMGUI_API void EndToolbar();
	IMGUI_API bool ToolbarButton(ImTextureID texture, const ImVec4& bg_color, const char* tooltip);
}