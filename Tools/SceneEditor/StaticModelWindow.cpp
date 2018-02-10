#include "StaticModelWindow.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "Commands/CustomCommand.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"

using namespace FireCube;

StaticModelWindow::StaticModelWindow(Engine *engine) : Object(engine)
{

}

void StaticModelWindow::Render(EditorState *editorState, StaticModelDescriptor *descriptor)
{
	std::string meshFileName = descriptor->GetMeshFilename();
	ImGui::InputText("", &meshFileName[0], meshFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	bool showFileOpen = ImGui::Button("...##meshOpenButton");
	ImGui::SameLine();
	ImGui::Text("Mesh");
	const char* chosenPath = openDialog.chooseFileDialog(showFileOpen, Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Models").c_str(), nullptr, "Select a file", ImVec2(600, 400), ImVec2(100, 100));
	std::string newMeshFileName = chosenPath;
	if (newMeshFileName.empty() == false)
	{
		std::replace(newMeshFileName.begin(), newMeshFileName.end(), '/', '\\');

		if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), newMeshFileName))
		{
			newMeshFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), newMeshFileName);
		}
		else
		{
			AssetUtils::ImportMesh(engine, newMeshFileName);
			newMeshFileName = "Models" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(newMeshFileName);
		}

		std::string oldMeshFileName = descriptor->GetMeshFilename();
		//meshFilePicker->SetPath(newMeshFileName);
		auto command = new CustomCommand(editorState, "Change Mesh", [descriptor, newMeshFileName, this]()
		{
			descriptor->SetMeshFilename(newMeshFileName, engine);
			descriptor->componentChanged(nullptr);
		}, [descriptor, oldMeshFileName, this]()
		{
			if (oldMeshFileName.empty() == false)
			{
				descriptor->SetMeshFilename(oldMeshFileName, engine);
				descriptor->componentChanged(nullptr);
			}
		});

		editorState->ExecuteCommand(command);
	}

	castShadowCheckBox.Render("Cast Shadow", editorState, "Change Cast Shadow", [descriptor]() {
		return descriptor->GetCastShadow();
	}, [descriptor](bool value) {
		descriptor->SetCastShadow(value);
	});

	lightMaskInput.Render("Light Mask", editorState, "Change Mask", [descriptor]() {
		return descriptor->GetLightMask();
	}, [descriptor](unsigned int value) {
		descriptor->SetLightMask(value);
	});

	collisionQueryMaskInput.Render("Collision Query Mask", editorState, "Change Mask", [descriptor]() {
		return descriptor->GetCollisionQueryMask();
	}, [descriptor](unsigned int value) {
		descriptor->SetCollisionQueryMask(value);
	});
}
