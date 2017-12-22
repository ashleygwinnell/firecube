#include "ImguiHelpers.h"
#include "imgui.h"
#include "EditorState.h"

using namespace FireCube;

Vec3InputHelper::Vec3InputHelper() : prevCommand(nullptr), isActive(false), editorState(nullptr)
{

}

void Vec3InputHelper::Render()
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

void Vec3InputHelper::Init(const std::string &label, EditorState *editorState, std::function<FireCube::vec3()> getValue, std::function<Command *(FireCube::vec3, FireCube::vec3)> setValue)
{
	this->label = label;
	this->editorState = editorState;
	this->getValue = getValue;
	this->setValue = setValue;
}
