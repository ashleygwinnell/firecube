#include "HierarchyWindow.h"
#include <imgui.h>
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"
#include "SceneWriter.h"

using namespace FireCube;

HierarchyWindow::HierarchyWindow(Engine *engine) : Object(engine), rootDesc(nullptr)
{

}

void HierarchyWindow::Render()
{
	ImGui::SetNextDock(ImGuiDockSlot_Left);	
	if (ImGui::BeginDock("Hierarchy", nullptr, 0, ImVec2(50, -1)))
	{
		RenderChildren(rootDesc);
	}
	ImGui::EndDock();
}

void HierarchyWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;
}

void HierarchyWindow::RenderChildren(NodeDescriptor *root)
{	
	for (auto child : root->GetChildren())
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (editorState->GetSelectedNode() == child ? ImGuiTreeNodeFlags_Selected : 0);
		if (child->GetChildren().empty())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}
		
		ImGui::PushID(child);
		if (child->IsPrefab())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.0f, 0.0));
		}
		bool nodeOpen = ImGui::TreeNodeEx(child->GetName().c_str(), nodeFlags);
		if (child->IsPrefab())
		{
			ImGui::PopStyleColor();
		}
		if (ImGui::BeginPopupContextItem("context menu"))
		{
			if (child->IsPrefab())
			{
				if (ImGui::Selectable("Update Prefab"))
				{
					std::string tragetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + child->GetPrefabPath();
					SceneWriter sceneWriter;
					sceneWriter.SerializePrefab(child, tragetPath);
				}
				if (ImGui::Selectable("Unlink From Prefab"))
				{
					child->SetIsPrefab(false);
				}
			}
			else
			{
				if (ImGui::Selectable("Create Prefab"))
				{
					std::string tragetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Prefabs" + Filesystem::PATH_SEPARATOR + child->GetName() + ".xml";
					SceneWriter sceneWriter;
					sceneWriter.SerializePrefab(child, tragetPath);
					child->SetIsPrefab(true);
					child->SetPrefabPath("Prefabs" + Filesystem::PATH_SEPARATOR + child->GetName() + ".xml");
				}
			}

			ImGui::EndPopup();
		}
		ImGui::PopID();
		if (ImGui::IsItemClicked())
		{
			editorState->SetSelectedNode(child);
		}

		if (nodeOpen && child->GetChildren().empty() == false)
		{
			RenderChildren(child);
			ImGui::TreePop();
		}		
	}
}
