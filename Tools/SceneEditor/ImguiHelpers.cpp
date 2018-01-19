#include <iostream>
#include "ImguiHelpers.h"
#include "imgui.h"
#include "EditorState.h"
#include "Commands/CustomCommand.h"

using namespace FireCube;

Vec3InputHelper::Vec3InputHelper() : prevCommand(nullptr), isActive(false)
{

}

void Vec3InputHelper::Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec3()> getValue, std::function<Command *(FireCube::vec3, FireCube::vec3)> setValue)
{
	vec3 value = getValue();
	float v[3] = { value.x, value.y, value.z };	
	ImGui::InputFloat3(label.c_str(), v, 2, ImGuiInputTextFlags_NoUndoRedo);
	if (v[0] != value.x || v[1] != value.y || v[2] != value.z)
	{
		vec3 newVal = vec3(v[0], v[1], v[2]);
		Command *command = setValue(newVal, prevValue);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValue = getValue();
		}
		isActive = true;
	}
	else
	{
		if (isActive)
		{
			prevCommand = nullptr;
		}

		isActive = false;
	}
}

void Vec3InputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec3()> getValue, std::function<void(FireCube::vec3)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](vec3 newValue, vec3 prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}

HexInputHelper::HexInputHelper() : prevCommand(nullptr), isActive(false)
{

}

void HexInputHelper::Render(const std::string &label, EditorState *editorState, std::function<unsigned int()> getValue, std::function<Command *(unsigned int, unsigned int)> setValue)
{
	unsigned int value = getValue();
	
	std::stringstream stream;
	stream << std::uppercase << std::hex << value;
	std::string result(stream.str());
	strcpy_s(val, 9, result.c_str());

	ImGui::InputText(label.c_str(), val, 9, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_NoUndoRedo);
	if (strcmp(val, result.c_str()) != 0)
	{
		unsigned int newVal;
		std::stringstream ss;
		ss << std::uppercase << std::hex << val;
		ss >> newVal;
		Command *command = setValue(newVal, prevValue);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValue = getValue();
		}
		isActive = true;
	}
	else
	{
		if (isActive)
		{
			prevCommand = nullptr;
		}

		isActive = false;
	}
}

void HexInputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<unsigned int()> getValue, std::function<void(unsigned int)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](unsigned int newValue, unsigned int prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});	
}

FloatInputHelper::FloatInputHelper() : prevCommand(nullptr), isActive(false)
{

}

void FloatInputHelper::Render(const std::string &label, EditorState *editorState, std::function<float()> getValue, std::function<Command *(float, float)> setValue)
{
	float value = getValue();
	float v = value;
	
	ImGui::InputFloat(label.c_str(), &v, 0.0f, 0.0f, -1, ImGuiInputTextFlags_NoUndoRedo);
	if (v != value)
	{		
		Command *command = setValue(v, prevValue);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValue = getValue();
		}
		isActive = true;
	}
	else
	{
		if (isActive)
		{
			prevCommand = nullptr;
		}

		isActive = false;
	}
}

void FloatInputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<float()> getValue, std::function<void(float)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](float newValue, float prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}

CheckBoxHelper::CheckBoxHelper() : prevCommand(nullptr), isActive(false)
{

}

void CheckBoxHelper::Render(const std::string &label, EditorState *editorState, std::function<bool()> getValue, std::function<Command *(bool, bool)> setValue)
{
	bool curValue = getValue();
	bool value = curValue;
	
	ImGui::Checkbox(label.c_str(), &value);
	if (curValue != value)
	{
		Command *command = setValue(value, prevValue);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValue = getValue();
		}
		isActive = true;
	}
	else
	{
		if (isActive)
		{
			prevCommand = nullptr;
		}

		isActive = false;
	}
}

void CheckBoxHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<bool()> getValue, std::function<void(bool)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](bool newValue, bool prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}

ColorInputHelper::ColorInputHelper() : prevCommand(nullptr), isActive(false)
{

}

void ColorInputHelper::Render(const std::string &label, EditorState *editorState, std::function<vec3()> getValue, std::function<Command *(vec3, vec3)> setValue)
{
	vec3 value = getValue();

	float color[3] = { value.x, value.y, value.z };
	bool  modified = ImGui::ColorEdit3(label.c_str(), color);
	
	if (modified)
	{
		vec3 newVal = vec3(color[0], color[1], color[2]);
		Command *command = setValue(newVal, prevValue);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValue = getValue();
		}
		isActive = true;
	}
	else
	{
		if (isActive)
		{
			prevCommand = nullptr;
		}

		isActive = false;
	}
}

void ColorInputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<vec3()> getValue, std::function<void(vec3)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](vec3 newValue, vec3 prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}

TextInputHelper::TextInputHelper() : prevCommand(nullptr), isActive(false)
{

}

void TextInputHelper::Render(const std::string &label, EditorState *editorState, std::function<std::string()> getValue, std::function<Command *(const std::string &, const std::string &)> setValue)
{
	std::string value = getValue();	
	strcpy_s(val, 256, value.c_str());
	
	ImGui::InputText(label.c_str(), val, 256, ImGuiInputTextFlags_NoUndoRedo);	
	if (strcmp(val, value.c_str()) != 0)
	{		
		Command *command = setValue(val, prevValue);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValue = getValue();
		}
		isActive = true;
	}
	else
	{
		if (isActive)
		{
			prevCommand = nullptr;
		}

		isActive = false;
	}
}

void TextInputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<std::string()> getValue, std::function<void(const std::string &)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](const std::string &newValue, const std::string &prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}