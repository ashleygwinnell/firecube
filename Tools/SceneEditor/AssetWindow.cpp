#include "AssetWindow.h"
#include "imgui.h"
#include "EditorState.h"

using namespace FireCube;

AssetWindow::AssetWindow(Engine *engine) : Object(engine), isOpen(true)
{

}

void AssetWindow::Render()
{
	ImGui::SetNextDock(ImGuiDockSlot_Right);
	if (ImGui::BeginDock("Asset", &isOpen))
	{
		if (assetType == AssetType::TEXTURE)
		{
			ImGui::Text(currentAsset.c_str());
			if (currentAsset.empty() == false)
			{
				if (ImGui::Button("Reload"))
				{
					auto texture = engine->GetResourceCache()->GetResource<Texture2D>(currentAsset);
					auto image = engine->GetResourceCache()->GetResource<Image>(currentAsset);
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
		}
		
	}
	ImGui::EndDock();
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
}
