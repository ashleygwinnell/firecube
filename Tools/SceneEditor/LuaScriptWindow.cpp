#include <fstream>
#include <regex>
#include "LuaScriptWindow.h"
#include "Descriptors/LuaScriptDescriptor.h"
#include "Commands/CustomCommand.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"

using namespace FireCube;

LuaScriptWindow::LuaScriptWindow(Engine *engine) : Object(engine), lastDescriptor(nullptr)
{

}

void LuaScriptWindow::Render(EditorState *editorState, LuaScriptDescriptor *descriptor)
{
	std::string selectedPath;
	std::string scriptFileName = descriptor->GetScriptFilename();
	ImGui::BeginGroup();
	ImGui::InputText("", &scriptFileName[0], scriptFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("...##scriptOpenButton"))
	{
		ImGuiHelpers::ShowAssetSelectionPopup("Select Script");
	}
	ImGui::SameLine();
	ImGui::Text("Script");
	ImGui::EndGroup();
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset");
		if (payload)
		{
			AssetType type;
			std::string path;
			AssetUtils::DeserializeAssetDescription((const char *)payload->Data, type, path);
			std::replace(path.begin(), path.end(), '\\', '/');
			path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

			if (type == AssetType::SCRIPT)
			{
				std::string oldScriptFileName = descriptor->GetScriptFilename();
				std::string oldObjectName = descriptor->GetObjectName();

				std::string newObjectName = GetObjectNameFromScript(Filesystem::JoinPath(Filesystem::GetAssetsFolder(), path));

				auto command = new CustomCommand(editorState, "Change Script", [descriptor, path, newObjectName]()
				{
					descriptor->SetScriptFilename(path);
					if (newObjectName.empty() == false)
					{
						descriptor->SetObjectName(newObjectName);
					}
					descriptor->componentChanged(nullptr);
				}, [descriptor, oldScriptFileName, oldObjectName]()
				{
					descriptor->SetScriptFilename(oldScriptFileName);
					descriptor->SetObjectName(oldObjectName);
					descriptor->componentChanged(nullptr);
				});

				editorState->ExecuteCommand(command);
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGuiHelpers::AssetSelectionPopup("Select Script", AssetType::SCRIPT, selectedPath))
	{
		std::string oldScriptFileName = descriptor->GetScriptFilename();
		std::string oldObjectName = descriptor->GetObjectName();
		std::vector<std::pair<std::string, std::string>> oldProperties = descriptor->GetProperties();
		std::string newObjectName = GetObjectNameFromScript(Filesystem::JoinPath(Filesystem::GetAssetsFolder(), selectedPath));
		auto command = new CustomCommand(editorState, "Change Script", [this, descriptor, selectedPath, newObjectName]()
		{
			descriptor->SetScriptFilename(selectedPath);
			if (newObjectName.empty() == false)
			{
				descriptor->SetObjectName(newObjectName);
				UpdateScriptProperties(selectedPath, newObjectName);
				for (auto &property : properties)
				{
					descriptor->SetProperty(property.name, property.defaultValue);
				}
			}

			descriptor->componentChanged(nullptr);
		}, [descriptor, oldScriptFileName, oldObjectName, oldProperties]()
		{
			descriptor->SetScriptFilename(oldScriptFileName);
			descriptor->SetObjectName(oldObjectName);
			descriptor->SetProperties(oldProperties);
			descriptor->componentChanged(nullptr);
		});

		editorState->ExecuteCommand(command);
	}

	objectNameInput.Render("Object", editorState, "Change Object", [descriptor]() {
		return descriptor->GetObjectName();
	}, [descriptor](const std::string &newValue) {
		descriptor->SetObjectName(newValue);
	});

	ImGui::Columns(2, "luaScriptPropertiesColumns", true);
	
	for (auto &property : properties)
	{
		std::string propertyName = property.name;
		std::string curValue = descriptor->GetProperty(propertyName);
		ImGui::Text(propertyName.c_str());
		ImGui::NextColumn();
		ImGui::PushID(propertyName.c_str());
		switch (property.type)
		{
		case ScriptPropertyType::STRING:
			property.stringInput.Render("", editorState, "Change Value", [curValue]() {
				return curValue;
			}, [propertyName, descriptor](const std::string &newValue) {
				descriptor->SetProperty(propertyName, newValue);
			});
			break;
		case ScriptPropertyType::BOOL:
		{
			bool curVal = Variant::FromString(curValue).GetBool();
			property.boolInput.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](bool newValue) {
				descriptor->SetProperty(propertyName, Variant(newValue).ToString());
			});
			break;
		}
		case ScriptPropertyType::FLOAT:
		{
			float curVal = Variant::FromString(curValue).GetFloat();
			property.floatInput.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](float newValue) {
				descriptor->SetProperty(propertyName, std::to_string(newValue));
			});
			break;
		}
		case ScriptPropertyType::VEC2:
		{
			vec2 curVal = Variant::FromString(curValue).GetVec2();
			property.vec2Input.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](vec2 newValue) {
				descriptor->SetProperty(propertyName, Variant(newValue).ToString());
			});
			break;
		}
		case ScriptPropertyType::VEC3:
		{
			vec3 curVal = Variant::FromString(curValue).GetVec3();
			property.vec3Input.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](vec3 newValue) {
				descriptor->SetProperty(propertyName, Variant(newValue).ToString());
			});
			break;
		}
		case ScriptPropertyType::VEC4:
		{
			vec4 curVal = Variant::FromString(curValue).GetVec4();
			property.vec4Input.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](vec4 newValue) {
				descriptor->SetProperty(propertyName, Variant(newValue).ToString());
			});
			break;
		}
		case ScriptPropertyType::RGB:
		{
			vec3 curVal = Variant::FromString(curValue).GetVec3();
			property.colorInput.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](vec3 newValue) {
				descriptor->SetProperty(propertyName, Variant(newValue).ToString());
			});
			break;
		}
		case ScriptPropertyType::RGBA:
		{
			vec4 curVal = Variant::FromString(curValue).GetVec4();
			property.colorInput.Render("", editorState, "Change Value", [curVal]() {
				return curVal;
			}, [propertyName, descriptor](vec4 newValue) {
				descriptor->SetProperty(propertyName, Variant(newValue).ToString());
			});
			break;
		}
		default:
			break;
		}
		ImGui::PopID();
		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	if (lastDescriptor != descriptor)
	{
		lastDescriptor = descriptor;
		UpdateScriptProperties(descriptor->GetScriptFilename(), descriptor->GetObjectName());
		for (auto &property : properties)
		{
			if (!descriptor->HasProperty(property.name))
			{
				descriptor->SetProperty(property.name, property.defaultValue);
			}
		}
	}
}

std::string LuaScriptWindow::GetObjectNameFromScript(const std::string &filename)
{
	std::ifstream file(filename);
	const std::string code((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	std::regex rgx("\\s*(\\w+)\\s*=\\s*Script");
	std::smatch match;

	if (std::regex_search(code.begin(), code.end(), match, rgx))
	{
		return match[1];
	}

	return "";
}

void LuaScriptWindow::UpdateScriptProperties(const std::string &filename, const std::string &objectName)
{
	this->properties.clear();

	if (filename.empty() || objectName.empty())
	{
		return;
	}

	LuaFile *luaFile = engine->GetResourceCache()->GetResource<LuaFile>(filename);
	luaFile->SetExecuted(false);
	engine->GetLuaState()->ExecuteFile(luaFile);	
	LuaFunction *getProperties = engine->GetLuaState()->GetFunction(objectName + ".GetProperties");
	if (getProperties)
	{
		auto propertiesObject = (*getProperties)();
		sol::table properties = propertiesObject.as<sol::table>();
		
		std::size_t count = properties.size();
		for (std::size_t i = 1; i <= count; ++i)
		{
			sol::table property = properties[i].get<sol::table>();
			std::string propertyName = property["name"].get<std::string>();
			this->properties.emplace_back();
			Property &prop = this->properties.back();
			prop.name = propertyName;
			prop.type = static_cast<ScriptPropertyType>(property["type"].get<unsigned int>());
			sol::object defaultValue = property["default"];
			if (defaultValue != sol::nil)
			{
				switch (prop.type)
				{
				case ScriptPropertyType::STRING:
					prop.defaultValue = defaultValue.as<std::string>();
					break;
				case ScriptPropertyType::BOOL:
					prop.defaultValue = Variant(defaultValue.as<bool>()).ToString();
					break;
				case ScriptPropertyType::FLOAT:
					prop.defaultValue = Variant(defaultValue.as<float>()).ToString();
					break;
				case ScriptPropertyType::VEC2:
					prop.defaultValue = Variant(defaultValue.as<vec2>()).ToString();
					break;
				case ScriptPropertyType::VEC3:
					prop.defaultValue = Variant(defaultValue.as<vec2>()).ToString();
					break;
				case ScriptPropertyType::VEC4:
					prop.defaultValue = Variant(defaultValue.as<vec4>()).ToString();
					break;
				case ScriptPropertyType::RGB:
					prop.defaultValue = Variant(defaultValue.as<vec3>()).ToString();
					break;
				case ScriptPropertyType::RGBA:
					prop.defaultValue = Variant(defaultValue.as<vec4>()).ToString();
					break;
				default:
					break;
				}
			}
		}
	}
}
