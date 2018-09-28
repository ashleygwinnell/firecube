#include <iostream>
#include "ImguiHelpers.h"
#include "imgui.h"
#include "EditorState.h"
#include "Commands/CustomCommand.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

using namespace FireCube;

Vec2InputHelper::Vec2InputHelper() : prevCommand(nullptr), isActive(false)
{

}

void Vec2InputHelper::Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec2()> getValue, std::function<Command *(FireCube::vec2, FireCube::vec2)> setValue)
{
	vec2 value = getValue();
	float v[2] = { value.x, value.y};
	ImGui::InputFloat2(label.c_str(), v, 2, ImGuiInputTextFlags_NoUndoRedo);
	if (v[0] != value.x || v[1] != value.y)
	{
		vec2 newVal = vec2(v[0], v[1]);
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

void Vec2InputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec2()> getValue, std::function<void(FireCube::vec2)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](vec2 newValue, vec2 prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}

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

Vec4InputHelper::Vec4InputHelper() : prevCommand(nullptr), isActive(false)
{

}

void Vec4InputHelper::Render(const std::string &label, EditorState *editorState, std::function<FireCube::vec4()> getValue, std::function<Command *(FireCube::vec4, FireCube::vec4)> setValue)
{
	vec4 value = getValue();
	float v[4] = { value.x, value.y, value.z, value.w };
	ImGui::InputFloat4(label.c_str(), v, 2, ImGuiInputTextFlags_NoUndoRedo);
	if (v[0] != value.x || v[1] != value.y || v[2] != value.z || v[3] != value.w)
	{
		vec4 newVal(v[0], v[1], v[2], v[3]);
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

void Vec4InputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<FireCube::vec4()> getValue, std::function<void(FireCube::vec4)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](vec4 newValue, vec4 prevValue) -> Command * {
		return new CustomCommand(editorState, description, [setValue, newValue]() {
			setValue(newValue);
		}, [setValue, prevValue]() {
			setValue(prevValue);
		});
	});
}

UIntInputHelper::UIntInputHelper() : prevCommand(nullptr), isActive(false)
{

}

void UIntInputHelper::Render(const std::string &label, EditorState *editorState, std::function<unsigned int()> getValue, std::function<Command *(unsigned int, unsigned int)> setValue)
{
	unsigned int value = getValue();

	std::stringstream stream;
	stream << value;
	std::string result(stream.str());
	strcpy_s(val, 9, result.c_str());

	ImGui::InputText(label.c_str(), val, 9, ImGuiInputTextFlags_NoUndoRedo);
	if (strcmp(val, result.c_str()) != 0)
	{
		unsigned int newVal;
		std::stringstream ss;
		ss << val;
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

void UIntInputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<unsigned int()> getValue, std::function<void(unsigned int)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](unsigned int newValue, unsigned int prevValue) -> Command * {
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

void ColorInputHelper::Render(const std::string &label, EditorState *editorState, std::function<vec4()> getValue, std::function<Command *(vec4, vec4)> setValue)
{
	vec4 value = getValue();

	float color[4] = { value.x, value.y, value.z, value.w };
	bool  modified = ImGui::ColorEdit4(label.c_str(), color);

	if (modified)
	{
		vec4 newVal = vec4(color[0], color[1], color[2], color[3]);
		Command *command = setValue(newVal, prevValueVec4);
		editorState->ExecuteCommand(command, prevCommand);
		prevCommand = command;
	}

	if (ImGui::IsItemActive())
	{
		if (isActive == false)
		{
			prevValueVec4 = getValue();
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

void ColorInputHelper::Render(const std::string &label, EditorState *editorState, const std::string &description, std::function<vec4()> getValue, std::function<void(vec4)> setValue)
{
	Render(label, editorState, getValue, [editorState, description, setValue](vec4 newValue, vec4 prevValue) -> Command * {
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

namespace ImGui
{

	bool ToolbarButton(ImTextureID texture, const ImVec4& bg_color, const char* tooltip)
	{
		auto frame_padding = ImGui::GetStyle().FramePadding;
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

		bool ret = false;
		ImGui::SameLine();
		ImVec4 tint_color = ImGui::GetStyle().Colors[ImGuiCol_Text];
		if (ImGui::ImageButton(texture, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, bg_color, tint_color))
		{
			ret = true;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s", tooltip);
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(3);
		return ret;
	}

	bool ToolbarButton(const char *label, const char* tooltip)
	{
		auto frame_padding = ImGui::GetStyle().FramePadding;
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

		bool ret = false;
		ImGui::SameLine();
		ImVec4 tint_color = ImGui::GetStyle().Colors[ImGuiCol_Text];
		if (ImGui::Button(label, ImVec2(24, 24)))
		{
			ret = true;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s", tooltip);
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(3);
		return ret;
	}

	bool BeginToolbar(const char* str_id, ImVec2 screen_pos, ImVec2 size)
	{
		bool is_global = GImGui->CurrentWindowStack.Size == 1;
		SetNextWindowPos(screen_pos);
		ImVec2 frame_padding = GetStyle().FramePadding;
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
		PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		float padding = frame_padding.y * 2;
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
		if (size.x == 0) size.x = GetContentRegionAvailWidth();
		SetNextWindowSize(size);

		bool ret = is_global ? Begin(str_id, nullptr, flags) : BeginChild(str_id, size, false, flags);
		PopStyleVar(3);

		return ret;
	}


	void EndToolbar()
	{
		auto frame_padding = ImGui::GetStyle().FramePadding;
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
		PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImVec2 pos = GetWindowPos();
		ImVec2 size = GetWindowSize();
		if (GImGui->CurrentWindowStack.Size == 2) End(); else EndChild();
		PopStyleVar(3);
		ImGuiWindow* win = GetCurrentWindowRead();
		if (GImGui->CurrentWindowStack.Size > 1) SetCursorScreenPos(pos + ImVec2(0, size.y + GetStyle().FramePadding.y * 2));
	}
}

struct ItemInfo
{
	std::string label;
	std::string relPath;
	std::string absPath;
	bool isDirectory;
};

std::vector<ItemInfo> GetItemsInPath(const std::string &path)
{
	std::string basePath = Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), path) ? Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path) : 
		Filesystem::MakeRelativeTo(Filesystem::GetCoreDataFolder(), path);	
	std::vector<ItemInfo> ret;
	std::string pattern(path);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string p = data.cFileName;
			if (p != "." && p != "..")
			{
				ItemInfo item;
				item.isDirectory = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
				item.label = item.isDirectory ? "[" + Filesystem::GetLastPathComponent(p) + "]" : Filesystem::GetLastPathComponent(p);				
				item.relPath = Filesystem::JoinPath(basePath, p);
				item.absPath = Filesystem::JoinPath(path, p);				
				ret.push_back(item);
			}
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}

	return ret;
}

std::string GetAssetsPath(AssetType type)
{
	switch (type)
	{
	case AssetType::MATERIAL:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Materials");
		break;
	case AssetType::TEXTURE:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Textures");
		break;
	case AssetType::SHADER:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Shaders");
		break;
	case AssetType::TECHNIQUE:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Techniques");
		break;
	case AssetType::MESH:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Models");
		break;
	case AssetType::SOUND:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Sounds");
		break;
	case AssetType::SCRIPT:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Scripts");
		break;
	case AssetType::SCENE:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Scenes");
		break;
	case AssetType::PREFAB:
		return Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Prefabs");
		break;
	default:
		break;
	}

	return "";
}

std::string GetCoreAssetsPath(AssetType type)
{
	switch (type)
	{
	case AssetType::MATERIAL:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Materials");
		break;
	case AssetType::TEXTURE:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Textures");
		break;
	case AssetType::SHADER:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Shaders");
		break;
	case AssetType::TECHNIQUE:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Techniques");
		break;
	case AssetType::MESH:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Models");
		break;
	case AssetType::SOUND:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Sounds");
		break;
	case AssetType::SCRIPT:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Scripts");
		break;
	case AssetType::SCENE:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Scenes");
		break;
	case AssetType::PREFAB:
		return Filesystem::JoinPath(Filesystem::GetCoreDataFolder(), "Prefabs");
		break;
	default:
		break;
	}

	return "";
}

void ImGuiHelpers::ShowAssetSelectionPopup(const std::string &title)
{
	ImGui::OpenPopup(title.c_str());
}

bool ImGuiHelpers::AssetSelectionPopup(const std::string &title, AssetType type, std::string &selectedPath)
{
	if (!ImGui::IsPopupOpen(title.c_str()))
	{
		return false;
	}

	static AssetType curType = AssetType::UNKNOWN;
	static std::string curBasePath;
	static std::string curPath;
	static std::string lastPath;
	static std::vector<ItemInfo> curItems;
	bool ret = false;
	if (curType != type || lastPath != curPath)
	{
		if (curType != type)
		{
			curType = type;
			curBasePath = curPath = lastPath = GetAssetsPath(type);
		}

		if (lastPath != curPath)
		{
			lastPath = curPath;
		}
		curItems = GetItemsInPath(curPath);
		if (curPath == curBasePath)
		{
			if (type == AssetType::MATERIAL || type == AssetType::SHADER || type == AssetType::TECHNIQUE)
			{
				auto extraItems = GetItemsInPath(GetCoreAssetsPath(type));
				for (auto &extraItem : extraItems)
				{
					if (std::find_if(curItems.begin(), curItems.end(), [&extraItem](const ItemInfo &item) {
						return item.relPath == extraItem.relPath;
					}) == curItems.end())
					{
						curItems.push_back(extraItem);
					}
				}
			}
		}

		std::sort(curItems.begin(), curItems.end(), [](const ItemInfo &first, const ItemInfo &second) {
			if (first.isDirectory && !second.isDirectory)
			{
				return true;
			}
			else if (!first.isDirectory && second.isDirectory)
			{
				return false;
			}
			else
			{
				return first.label < second.label;
			}
		});
	}
	if (ImGui::BeginPopup(title.c_str()))
	{
		if (curPath != curBasePath)
		{
			if (ImGui::Selectable("..", false, ImGuiSelectableFlags_DontClosePopups))
			{
				curPath = Filesystem::GetDirectoryName(curPath);
			}
		}

		if (ImGui::Selectable("[None]"))
		{
			ret = true;
			selectedPath = "";
		}

		for (auto &item : curItems)
		{
			if (item.isDirectory)
			{
				if (ImGui::Selectable(item.label.c_str(), false, ImGuiSelectableFlags_DontClosePopups))
				{
					curPath = item.absPath;
				}
			}
			else
			{
				if (ImGui::Selectable(item.label.c_str()))
				{
					ret = true;
					selectedPath = item.relPath;
				}
			}
		}
		ImGui::EndPopup();
	}

	return ret;
}
