#include "PlaneWindow.h"
#include "Descriptors/PlaneDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"
#include "AssetUtils.h"

using namespace FireCube;

PlaneWindow::PlaneWindow(FireCube::Engine *engine) : Object(engine)
{

}

void PlaneWindow::Render(EditorState *editorState, PlaneDescriptor *descriptor)
{
	std::string selectedPath;
	sizeInput.Render("Size", editorState, "Change Size", [descriptor]() {
		return descriptor->GetSize();
	}, [descriptor, this](vec2 newValue) {
		descriptor->SetSize(newValue, engine);
	});

	std::string materialFileName = descriptor->GetMaterialFileName();
	ImGui::InputText("", &materialFileName[0], materialFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("..."))
	{
		ImGuiHelpers::ShowAssetSelectionPopup("Select Material");
	}
	ImGui::SameLine();
	ImGui::Text("Material");
	if (ImGuiHelpers::AssetSelectionPopup("Select Material", AssetType::MATERIAL, selectedPath))
	{
		std::string oldValue = descriptor->GetMaterialFileName();

		auto command = new CustomCommand(editorState, "Change Material", [descriptor, selectedPath, this]()
		{
			descriptor->SetMaterialFileName(selectedPath, engine);
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

