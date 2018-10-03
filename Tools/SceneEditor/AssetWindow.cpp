#define NOMINMAX
#include "AssetWindow.h"
#include "imgui.h"
#include "EditorState.h"

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
	}
}

void AssetWindow::RenderTextureAsset()
{
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(currentAsset.c_str());
		if (ImGui::Button("Reload"))
		{
			if (texture)
			{
				engine->GetResourceCache()->ReloadResource(texture);
			}

			if (image)
			{
				engine->GetResourceCache()->ReloadResource(image);
			}
		}
	}

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

void AssetWindow::RenderMeshAsset()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(currentAsset.c_str());
		if (ImGui::Button("Reload"))
		{
			auto mesh = engine->GetResourceCache()->FindResource<Mesh>(currentAsset);

			if (mesh)
			{
				engine->GetResourceCache()->ReloadResource(mesh);
			}
		}

		if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto &material : meshMaterials)
			{
				if (ImGui::Selectable(material->GetName().c_str()))
				{
					editorState->materialPicked(editorState, material);
					editorState->showMaterialEditor(editorState);
				}
			}
		}
	}
}
