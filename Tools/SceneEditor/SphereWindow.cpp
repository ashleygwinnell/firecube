#include "SphereWindow.h"
#include "Descriptors/SphereDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"
#include "AssetUtils.h"

using namespace FireCube;

SphereWindow::SphereWindow(FireCube::Engine *engine) : Object(engine)
{

}

void SphereWindow::Render(EditorState *editorState, SphereDescriptor *descriptor)
{
	if (ImGui::CollapsingHeader("Sphere", ImGuiTreeNodeFlags_DefaultOpen))
	{
		radiusInput.Render("Radius", editorState, "Change Radius", [descriptor]() {
			return descriptor->GetRadius();
		}, [descriptor, this](float newValue) {
			descriptor->SetRadius(newValue, engine);
		});

		columnsInput.Render("Columns", editorState, "Change Columns", [descriptor]() {
			return descriptor->GetColumns();
		}, [descriptor, this](unsigned int newValue) {
			descriptor->SetColumns(newValue, engine);
		});

		ringsInput.Render("Rings", editorState, "Change Rings", [descriptor]() {
			return descriptor->GetRings();
		}, [descriptor, this](unsigned int newValue) {
			descriptor->SetRings(newValue, engine);
		});

		std::string materialFileName = descriptor->GetMaterialFileName();
		ImGui::InputText("", &materialFileName[0], materialFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		bool showFileOpen = ImGui::Button("...");
		ImGui::SameLine();
		ImGui::Text("Material");
		const char* chosenPath = openDialog.chooseFileDialog(showFileOpen, Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Models").c_str(), nullptr, "Select a file", ImVec2(600, 400), ImVec2(100, 100));
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
}

