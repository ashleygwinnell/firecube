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
	std::string selectedPath;
	ImGui::BeginGroup();
	ImGui::InputText("", &meshFileName[0], meshFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("...##meshOpenButton"))
	{
		ImGuiHelpers::ShowAssetSelectionPopup("Select Mesh");
	}
	ImGui::SameLine();
	ImGui::Text("Mesh");
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

			if (type == AssetType::MESH)
			{
				std::string oldMeshFileName = descriptor->GetMeshFilename();
				auto command = new CustomCommand(editorState, "Change Mesh", [descriptor, path, this]()
				{
					descriptor->SetMeshFilename(path, engine);
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
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGuiHelpers::AssetSelectionPopup("Select Mesh", AssetType::MESH, selectedPath))
	{
		std::string oldMeshFileName = descriptor->GetMeshFilename();
		auto command = new CustomCommand(editorState, "Change Mesh", [descriptor, selectedPath, this]()
		{
			descriptor->SetMeshFilename(selectedPath, engine);
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

	if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
	{
		StaticModel *staticModel = (StaticModel *)descriptor->GetComponent();
		unsigned int renderablePartsCount = staticModel->GetRenderableParts().size();
		auto renderablePartsMaterials = descriptor->GetRenderablePartsMaterials();
		for (unsigned int i = 0; i < renderablePartsCount; ++i)
		{
			ImGui::PushID(i);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 50.0f);
			ImGui::BeginGroup();
			std::string matName = "[None]";
			if (i < renderablePartsMaterials.size())
			{
				if (renderablePartsMaterials[i].empty() == false)
				{
					matName = renderablePartsMaterials[i];
				}
			}

			ImGui::Text(std::to_string(i).c_str());
			ImGui::SameLine();
			ImGui::InputText("", &matName[0], matName.size() + 1, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::Button("..."))
			{
				ImGuiHelpers::ShowAssetSelectionPopup("Select Material");
			}

			if (ImGuiHelpers::AssetSelectionPopup("Select Material", AssetType::MATERIAL, matName))
			{
				std::string oldMatName = descriptor->GetRenderablePartsMaterials().size() > i ? descriptor->GetRenderablePartsMaterials()[i] : "";
				auto command = new CustomCommand(editorState, "Change Material", [descriptor, matName, i, this]()
				{
					descriptor->SetRenderablePartMaterial(i, matName, engine);
				}, [descriptor, i, oldMatName, this]()
				{
					descriptor->SetRenderablePartMaterial(i, oldMatName, engine);
				});

				editorState->ExecuteCommand(command);
			}

			ImGui::EndGroup();
			ImGui::PopItemWidth();
			ImGui::PopID();
		}
	}
}
