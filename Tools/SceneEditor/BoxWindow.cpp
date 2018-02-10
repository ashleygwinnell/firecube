#include "BoxWindow.h"
#include "Descriptors/BoxDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"
#include "AssetUtils.h"

using namespace FireCube;

BoxWindow::BoxWindow(FireCube::Engine *engine) : Object(engine)
{

}

void BoxWindow::Render(EditorState *editorState, BoxDescriptor *descriptor)
{
	sizeInput.Render("Size", editorState, "Change Size", [descriptor]() {
		return descriptor->GetSize();
	}, [descriptor, this](vec3 newValue) {
		descriptor->SetSize(newValue, engine);
	});

	std::string materialFileName = descriptor->GetMaterialFileName();
	ImGui::InputText("", &materialFileName[0], materialFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	bool showFileOpen = ImGui::Button("...");
	ImGui::SameLine();
	ImGui::Text("Material");
	const char* chosenPath = openDialog.chooseFileDialog(showFileOpen, Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Materials").c_str(), nullptr, "Select a file", ImVec2(600, 400), ImVec2(100, 100));
	std::string newMaterialFileName = chosenPath;
	if (newMaterialFileName.empty() == false)
	{
		std::string newValue = AssetUtils::ImportMaterialIfNeeded(newMaterialFileName);
		std::string oldValue = descriptor->GetMaterialFileName();

		auto command = new CustomCommand(editorState, "Change Material", [descriptor, newValue, this]()
		{
			descriptor->SetMaterialFileName(newValue, engine);
			descriptor->componentChanged(nullptr);
		}, [descriptor, oldValue, this]()
		{
			descriptor->SetMaterialFileName(oldValue, engine);
			descriptor->componentChanged(nullptr);
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

