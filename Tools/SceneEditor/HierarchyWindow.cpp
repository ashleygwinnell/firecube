#include "HierarchyWindow.h"
#include <imgui.h>
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"
#include "SceneWriter.h"
#include "Commands/ReparentNodeCommand.h"

using namespace FireCube;

HierarchyWindow::HierarchyWindow(Engine *engine) : Object(engine), rootDesc(nullptr), isOpen(true), newSelectedNode(nullptr)
{

}

void HierarchyWindow::Render()
{
	ImGui::SetNextDock(ImGuiDockSlot_Left);	
	if (ImGui::BeginDock("Hierarchy", &isOpen))
	{
		ImGui::BeginChild("hierarchy_list");
		RenderChildren(rootDesc);
		ImGui::EndChild();
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("h_node");
			if (payload)
			{
				NodeDescriptor *draggedNode = *((NodeDescriptor **)payload->Data);
				if (draggedNode->GetParent() != rootDesc)
				{
					auto reparentNodeCommand = new ReparentNodeCommand(editorState, "Reparent", draggedNode, rootDesc);
					editorState->ExecuteCommand(reparentNodeCommand);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::EndDock();

	newSelectedNode = nullptr;
}

void HierarchyWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;

	SubscribeToEvent(editorState->selectedNodeChanged, &HierarchyWindow::SelectedNodeChanged);
}

bool *HierarchyWindow::GetIsOpenPtr()
{
	return &isOpen;
}

void HierarchyWindow::RenderChildren(NodeDescriptor *root)
{	
	auto children = root->GetChildren();
	for (auto child : children)
	{
		bool renderChildren = true;
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (editorState->GetSelectedNode() == child ? ImGuiTreeNodeFlags_Selected : 0);
		if (child->GetChildren().empty())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		{
			NodeDescriptor *t = newSelectedNode ? newSelectedNode->GetParent() : nullptr;
			while (t)
			{
				if (t == child)
				{
					ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
					break;
				}
				t = t->GetParent();
			}
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

		if (ImGui::IsItemActive())
		{
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("h_node", &child, sizeof(NodeDescriptor *));				
				ImGui::Text(child->GetName().c_str());
				ImGui::EndDragDropSource();
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("h_node");
			if (payload)
			{
				NodeDescriptor *draggedNode = *((NodeDescriptor ** )payload->Data);
				if (draggedNode != child && draggedNode->GetParent() != child)
				{
					auto reparentNodeCommand = new ReparentNodeCommand(editorState, "Reparent", draggedNode, child);
					editorState->ExecuteCommand(reparentNodeCommand);
					renderChildren = false;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (editorState->GetSelectedNode() == child && ImGui::IsWindowFocused())
		{
			if (engine->GetInputManager()->IsKeyPressed(Key::U, KeyModifier::LEFT_ALT) || engine->GetInputManager()->IsKeyPressed(Key::D, KeyModifier::LEFT_ALT))
			{				
				auto parent = child->GetParent();
				if (parent)
				{
					int childrenCount = parent->GetChildren().size();					
					int index = std::distance(parent->GetChildren().begin(), std::find(parent->GetChildren().begin(), parent->GetChildren().end(), child));					

					if ((engine->GetInputManager()->IsKeyPressed(Key::U) && index > 0) || (engine->GetInputManager()->IsKeyPressed(Key::D) && index < childrenCount - 1))
					{
						int newIndex;
						if (engine->GetInputManager()->IsKeyPressed(Key::U))
						{
							newIndex = index - 1;
						}
						else
						{
							newIndex = index + 1;
						}						
						parent->SetChildIndex(child, newIndex);						
					}
				}
			}
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

		if (renderChildren && nodeOpen && child->GetChildren().empty() == false)
		{
			RenderChildren(child);
			ImGui::TreePop();			
		}		
	}
}

void HierarchyWindow::SelectedNodeChanged(NodeDescriptor *node)
{
	newSelectedNode = node;
}
