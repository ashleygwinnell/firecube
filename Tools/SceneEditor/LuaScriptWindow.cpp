#include <fstream>
#include <regex>
#include "LuaScriptWindow.h"
#include "Descriptors/LuaScriptDescriptor.h"
#include "Commands/CustomCommand.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"

using namespace FireCube;

LuaScriptWindow::LuaScriptWindow(Engine *engine) : Object(engine)
{

}

void LuaScriptWindow::Render(EditorState *editorState, LuaScriptDescriptor *descriptor)
{
	if (ImGui::CollapsingHeader("LuaScript", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string scriptFileName = descriptor->GetScriptFilename();
		ImGui::InputText("", &scriptFileName[0], scriptFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		bool showFileOpen = ImGui::Button("...");
		ImGui::SameLine();
		ImGui::Text("Script");
		const char* chosenPath = openDialog.chooseFileDialog(showFileOpen, Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Scripts").c_str(), nullptr, "Select a file", ImVec2(600, 400), ImVec2(100, 100));
		std::string newScriptFileName = chosenPath;
		if (newScriptFileName.empty() == false)
		{
			std::replace(newScriptFileName.begin(), newScriptFileName.end(), '/', '\\');

			if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), newScriptFileName))
			{
				newScriptFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), newScriptFileName);
			}
			else
			{
				std::string filename = Filesystem::GetLastPathComponent(newScriptFileName);
				std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scripts";
				Filesystem::CreateFolder(targetPath);
				Filesystem::CopyPath(newScriptFileName, targetPath + Filesystem::PATH_SEPARATOR + filename);
				newScriptFileName = "Scripts" + Filesystem::PATH_SEPARATOR + filename;
			}

			std::string oldScriptFileName = descriptor->GetScriptFilename();			
			std::string oldObjectName = descriptor->GetObjectName();

			std::string newObjectName = GetObjectNameFromScript(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + newScriptFileName);

			auto command = new CustomCommand(editorState, "Change Script", [descriptor, newScriptFileName, newObjectName]()
			{
				descriptor->SetScriptFilename(newScriptFileName);
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

		objectNameInput.Render("Object", editorState, "Change Object", [descriptor]() {
			return descriptor->GetObjectName();
		}, [descriptor](const std::string &newValue) {
			descriptor->SetObjectName(newValue);
		});

		ImGui::Columns(3, "luaScriptPropertiesColumns", true);

		auto properties = descriptor->GetProperties();
		int idx = 0;
		for (auto &property : properties)
		{
			auto &propertyNameInput = propertyNameInputs[idx];
			auto &propertyValueInput = propertyValueInputs[idx];
			
			std::string propertyName = property.first;
			std::string propertyValue = property.second;
			
			ImGui::PushID(idx);
			ImGui::PushID("Name");
			propertyNameInput.Render("", editorState, [propertyName]() {
				return propertyName;
			}, [editorState, descriptor, idx](const std::string &newPropertyName, const std::string &oldPropertyName) {				
				return new CustomCommand(editorState, "Rename Property", [descriptor, newPropertyName, idx]()
				{					
					descriptor->RenameProperty(idx, newPropertyName);
					descriptor->componentChanged(nullptr);
				}, [descriptor, oldPropertyName, idx]()
				{				
					descriptor->RenameProperty(idx, oldPropertyName);
					descriptor->componentChanged(nullptr);
				});				
			});
			ImGui::PopID();			
			ImGui::NextColumn();			
			ImGui::PushID("Value");
			propertyValueInput.Render("", editorState, "Change Value", [propertyValue]() {
				return propertyValue;
			}, [&properties, propertyName, descriptor](const std::string &newValue) {
				descriptor->SetProperty(propertyName, newValue);
			});
			ImGui::PopID();
			ImGui::NextColumn();
			if (ImGui::Button("Remove"))
			{
				auto command = new CustomCommand(editorState, "Remove Property", [descriptor, propertyName]()
				{
					descriptor->RemoveProperty(propertyName);
					descriptor->componentChanged(nullptr);
				}, [descriptor, propertyName, propertyValue]()
				{
					descriptor->SetProperty(propertyName, propertyValue);
					descriptor->componentChanged(nullptr);
				});

				editorState->ExecuteCommand(command);
			}

			ImGui::PopID();
			ImGui::NextColumn();
			++idx;
		}

		if (ImGui::Button("Add")) 
		{
			auto command = new CustomCommand(editorState, "Add Property", [descriptor]()
			{
				descriptor->SetProperty("", "");
				descriptor->componentChanged(nullptr);
			}, [descriptor]()
			{
				descriptor->RemoveProperty("");
				descriptor->componentChanged(nullptr);
			});

			editorState->ExecuteCommand(command);			
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