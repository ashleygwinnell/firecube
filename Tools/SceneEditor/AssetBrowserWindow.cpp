#include "AssetBrowserWindow.h"
#include "imgui.h"
#include "EditorState.h"

using namespace FireCube;

AssetBrowserWindow::AssetBrowserWindow(Engine *engine) : Object(engine), selectedItem(nullptr)
{

}

void AssetBrowserWindow::Render()
{
	NodeDescriptor *selectedNode = editorState->GetSelectedNode();

	ImGui::SetNextDock(ImGuiDockSlot_Bottom);
	if (ImGui::BeginDock("Asset Browser", nullptr, 0, ImVec2(50, -1)))
	{
		ImGui::Columns(3, "assetBrowserColumns", true);
		
		auto assetsFolder = Filesystem::GetAssetsFolder();
		if (assetsFolder.empty() == false)
		{
			RenderDirectoryTree(assetsFolder);
		}
		ImGui::NextColumn();
		if (itemsInSelectedPath.empty() == false)
		{
			for (auto &item : itemsInSelectedPath)
			{
				if (ImGui::Selectable(item.label.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (ImGui::IsMouseDoubleClicked(0))
					{
						if (item.isDirectory)
						{
							selectedPath = item.path;
							selectedItem = nullptr;
							itemsInSelectedPath = GetItemsInPath(selectedPath);
							break;
						}
					}
					else
					{
						selectedItem = &item;
					}
				}
			}
		}
		ImGui::NextColumn();
		if (selectedItem)
		{
			ImGui::Text(selectedItem->label.c_str());
		}
	}
	ImGui::EndDock();
}

void AssetBrowserWindow::RenderDirectoryTree(const std::string &path)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (selectedPath == path ? ImGuiTreeNodeFlags_Selected : 0);
	std::vector<std::string> items = GetDirectoriesInPath(path);
	if (items.empty())
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	
	ImGui::PushID(path.c_str());
	std::string label;
	if (path == Filesystem::GetAssetsFolder())
	{
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
		label = "Assets";
	}
	else
	{
		label = Filesystem::GetLastPathComponent(path);
	}

	bool nodeOpen = ImGui::TreeNodeEx(label.c_str(), nodeFlags);
	ImGui::PopID();
	if (ImGui::IsItemClicked())
	{
		selectedPath = path;
		selectedItem = nullptr;
		itemsInSelectedPath = GetItemsInPath(path);
	}

	if (nodeOpen && items.empty() == false)
	{
		for (auto &p : items)
		{
			RenderDirectoryTree(p);
		}
		ImGui::TreePop();
	}	
}

void AssetBrowserWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;	
}

std::vector<std::string> AssetBrowserWindow::GetDirectoriesInPath(const std::string &path)
{
	std::vector<std::string> ret;
	std::string pattern(path);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::string p = data.cFileName;
				if (p != "." && p != "..")
				{
					ret.push_back(Filesystem::JoinPath(path, p));
				}
			}
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}

	return ret;
}

std::vector<AssetBrowserWindow::FileInfo> AssetBrowserWindow::GetItemsInPath(const std::string &path)
{
	std::vector<FileInfo> ret;
	std::string pattern(path);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string p = data.cFileName;
			if (p != "." && p != "..")
			{
				FileInfo item;
				item.path = Filesystem::JoinPath(path, p);
				item.label = Filesystem::GetLastPathComponent(p);
				item.isDirectory = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
				if (!item.isDirectory)
				{
					item.assetType = AssetUtils::GetAssetTypeByPath(item.path);
				}
				ret.push_back(item);
			}
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}

	return ret;
}