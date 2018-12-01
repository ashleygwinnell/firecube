#define NOMINMAX
#include "AssetWindow.h"
#include "imgui.h"
#include "EditorState.h"
#include "ImguiHelpers.h"

using namespace FireCube;

AssetWindow::AssetWindow(Engine *engine) : Object(engine), isOpen(true)
{

}

void AssetWindow::Render()
{
	if (!isOpen)
	{
		return;
	}

	if (ImGui::Begin("Asset", &isOpen))
	{
		if (currentAsset.empty() == false)
		{
			switch (assetType)
			{
			case AssetType::TEXTURE:
				RenderTextureAsset();
				break;
			case AssetType::MESH:
				RenderMeshAsset();
			}
		}
	}
	ImGui::End();
}

void AssetWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;

	SubscribeToEvent(editorState, editorState->assetSelected, &AssetWindow::AssetSelected);
}

bool *AssetWindow::GetIsOpenPtr()
{
	return &isOpen;
}

void AssetWindow::AssetSelected(const std::string &asset)
{
	currentAsset = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), asset);
	assetType = AssetUtils::GetAssetTypeByPath(asset);
	if (assetType == AssetType::TEXTURE)
	{
		texture = engine->GetResourceCache()->FindResource<Texture2D>(currentAsset);
		image = engine->GetResourceCache()->FindResource<Image>(currentAsset);
	}
	else if (assetType == AssetType::MESH)
	{
		FireCube::Mesh mesh(engine);
		mesh.Load(currentAsset);
		meshMaterials.clear();
		for (auto &material : mesh.GetMaterials())
		{
			meshMaterials.insert(material);
		}
		meshMaterialNames = mesh.GetMaterialNames();
		meshMetadata.Load(Filesystem::JoinPath(Filesystem::GetAssetsFolder(), currentAsset + ".metadata"));
	}
}

void AssetWindow::RenderTextureAsset()
{
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(currentAsset.c_str());
		if (texture)
		{
			ImVec2 size = ImGui::GetContentRegionAvail();
			int width = texture->GetWidth();
			int height = texture->GetHeight();
			float maxDimension = std::min(size.x, size.y);

			if (width > maxDimension || height > maxDimension)
			{
				if (width > height)
				{
					size.x = maxDimension;
					size.y = (float)height / (float)width * maxDimension;
				}
				else
				{
					size.y = maxDimension;
					size.x = (float)width / (float)height * maxDimension;
				}
			}
			else
			{
				size = ImVec2((float)width, (float)height);
			}

			ImGui::Image((void *)(texture->GetObjectId()), size, ImVec2(0, 1), ImVec2(1, 0));
		}
	}
}

void AssetWindow::RenderMeshAsset()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(currentAsset.c_str());

		if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto &materialName : meshMaterialNames)
			{
				ImGui::PushID(materialName.c_str());
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - (meshMetadata.HasMaterialMapping(materialName) ? 150.0f : 220.0f));
				ImGui::BeginGroup();
				std::string selectedPath = "[None]";
				if (meshMetadata.HasMaterialMapping(materialName))
				{
					selectedPath = meshMetadata.GetMaterialMapping(materialName);
				}

				ImGui::Text(materialName.c_str());
				ImGui::SameLine();
				ImGui::InputText("", &selectedPath[0], selectedPath.size() + 1, ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					ImGuiHelpers::ShowAssetSelectionPopup("Select Material");
				}

				if (!meshMetadata.HasMaterialMapping(materialName))
				{
					ImGui::SameLine();
					if (ImGui::Button("Extract"))
					{
						for (auto mat : meshMaterials)
						{
							if (mat->GetName() == materialName)
							{
								std::string targetMaterialPath = Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Materials", mat->GetName() + ".xml");
								AssetUtils::SerializeMaterial(mat, targetMaterialPath);
								meshMetadata.SetMaterialMapping(materialName, Filesystem::JoinPath("Materials", mat->GetName() + ".xml"));
								break;
							}
						}
					}
				}

				if (ImGuiHelpers::AssetSelectionPopup("Select Material", AssetType::MATERIAL, selectedPath))
				{
					if (selectedPath.empty())
					{
						meshMetadata.RemoveMaterialMapping(materialName);
					}
					else
					{
						meshMetadata.SetMaterialMapping(materialName, selectedPath);
					}
				}
				ImGui::EndGroup();
				ImGui::PopItemWidth();
				ImGui::PopID();
			}

			if (ImGui::Button("Save"))
			{
				meshMetadata.Save(Filesystem::JoinPath(Filesystem::GetAssetsFolder(), currentAsset + ".metadata"));
			}
		}
	}
}
