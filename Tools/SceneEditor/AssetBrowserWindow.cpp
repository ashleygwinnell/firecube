#include "AssetBrowserWindow.h"
#include "imgui.h"
#include "EditorState.h"

using namespace FireCube;

AssetBrowserWindow::AssetBrowserWindow(Engine *engine) : Object(engine)
{

}

void AssetBrowserWindow::Render()
{
	NodeDescriptor *selectedNode = editorState->GetSelectedNode();

	ImGui::SetNextDock(ImGuiDockSlot_Bottom);
	if (ImGui::BeginDock("Asset Browser", nullptr, 0, ImVec2(50, -1)))
	{
		ImGui::Columns(3, "assetBrowserColumns", true);
		ImGui::Separator();

		auto assetsFolder = Filesystem::GetAssetsFolder();
		if (assetsFolder.empty() == false)
		{
			RenderDirectoryTree(assetsFolder);
		}
		ImGui::NextColumn();
		if (filesInSelectedPath.empty() == false)
		{
			for (auto &file : filesInSelectedPath)
			{
				std::string label = Filesystem::GetLastPathComponent(file);
				if (ImGui::Selectable(label.c_str())) 
				{

				}
			}
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
	std::string label = path == Filesystem::GetAssetsFolder() ? "Assets" : Filesystem::GetLastPathComponent(path);
	bool nodeOpen = ImGui::TreeNodeEx(label.c_str(), nodeFlags);
	ImGui::PopID();
	if (ImGui::IsItemClicked())
	{
		selectedPath = path;
		filesInSelectedPath = GetFilesInPath(path);
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

std::vector<std::string> AssetBrowserWindow::GetFilesInPath(const std::string &path)
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
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				std::string p = data.cFileName;
				ret.push_back(Filesystem::JoinPath(path, p));
			}
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}

	return ret;
}