#include "HierarchyWindow.h"
#include <imgui.h>
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"

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
		bool nodeOpen = ImGui::TreeNodeEx(child->GetName().c_str(), nodeFlags);
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
