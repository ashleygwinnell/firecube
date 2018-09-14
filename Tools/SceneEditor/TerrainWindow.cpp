#include "TerrainWindow.h"
#include "Descriptors/TerrainDescriptor.h"
#include "Commands/CustomCommand.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"

using namespace FireCube;

TerrainWindow::TerrainWindow(Engine *engine) : Object(engine)
{

}

void TerrainWindow::Render(EditorState *editorState, TerrainDescriptor *descriptor)
{
	std::string heightmapFileName = descriptor->GetHeightmapFilename();
	std::string selectedPath;
	ImGui::BeginGroup();
	ImGui::InputText("", &heightmapFileName[0], heightmapFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("...##terrainOpenButton"))
	{
		ImGuiHelpers::ShowAssetSelectionPopup("Select Heightmap");
	}
	ImGui::SameLine();
	ImGui::Text("Heightmap");
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

			if (type == AssetType::TEXTURE)
			{
				std::string oldHeightmapFileName = descriptor->GetHeightmapFilename();
				auto command = new CustomCommand(editorState, "Change Heightmap", [descriptor, path, this]()
				{
					descriptor->SetHeightmapFilename(path, engine);
					descriptor->componentChanged(nullptr);
				}, [descriptor, oldHeightmapFileName, this]()
				{
					if (oldHeightmapFileName.empty() == false)
					{
						descriptor->SetHeightmapFilename(oldHeightmapFileName, engine);
						descriptor->componentChanged(nullptr);
					}
				});

				editorState->ExecuteCommand(command);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGuiHelpers::AssetSelectionPopup("Select Heightmap", AssetType::TEXTURE, selectedPath))
	{
		std::string oldHeightmapFileName = descriptor->GetHeightmapFilename();
		auto command = new CustomCommand(editorState, "Change Heightmap", [descriptor, selectedPath, this]()
		{
			descriptor->SetHeightmapFilename(selectedPath, engine);
			descriptor->componentChanged(nullptr);
		}, [descriptor, oldHeightmapFileName, this]()
		{
			if (oldHeightmapFileName.empty() == false)
			{
				descriptor->SetHeightmapFilename(oldHeightmapFileName, engine);
				descriptor->componentChanged(nullptr);
			}
		});

		editorState->ExecuteCommand(command);
	}

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

	verticesSpacingInput.Render("Spacing", editorState, "Change Spacing", [descriptor]() {
		return descriptor->GetVerticesSpacing();
	}, [descriptor, this](vec3 newValue) {
		descriptor->SetVerticesSpacing(newValue, engine);
	});

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
