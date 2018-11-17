#include "HierarchyWindow.h"
#include <imgui.h>
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"
#include "SceneWriter.h"
#include "Commands/ReparentNodeCommand.h"
#include "Commands/RenameNodeCommand.h"

using namespace FireCube;

HierarchyWindow::HierarchyWindow(Engine *engine) : Object(engine), rootDesc(nullptr), isOpen(true), newSelectedNode(nullptr), currentEditedNode(nullptr)
{

}

void HierarchyWindow::Render()
{
	if (!isOpen)
	{
		return;
	}

	if (ImGui::Begin("Hierarchy", &isOpen))
	{
		if (ImGui::IsWindowFocused() && engine->GetInputManager()->IsKeyPressed(Key::ESCAPE))
		{
			currentEditedNode = nullptr;
		}

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
	ImGui::End();

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
		bool childHasChildren = child->GetChildren().empty() == false;
		bool renderChildren = true;
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (editorState->GetSelectedNode() == child ? ImGuiTreeNodeFlags_Selected : 0);
		if (!childHasChildren)
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

		static char nodeName[1024];
		std::string treeNodeLabel;

		if (currentEditedNode == child)
		{
			treeNodeLabel = "##" + child->GetName();
		}
		else
		{
			treeNodeLabel = child->GetName();
		}

		bool nodeOpen = ImGui::TreeNodeEx(treeNodeLabel.c_str(), nodeFlags);
		if (currentEditedNode == child)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
			ImGui::SameLine();
			if (startRenaming)
			{
				ImGui::SetKeyboardFocusHere();
				startRenaming = false;
			}
			
			bool ret = ImGui::InputText("##edit_node_name", nodeName, 1024, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

			if (ret || ImGui::IsItemDeactivated() == true)
			{
				if (std::string(nodeName) != child->GetName())
				{
					auto command = new RenameNodeCommand(editorState, "Rename", child, nodeName);
					editorState->ExecuteCommand(command);
				}
				currentEditedNode = nullptr;
			}

			ImGui::PopStyleVar();
		}

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
		if (ImGui::IsItemClicked() && currentEditedNode != child)
		{
			currentEditedNode = nullptr;
			editorState->SetSelectedNode(child);
		}
		
 		if (ImGui::IsWindowFocused()&& editorState->GetSelectedNode() == child && engine->GetInputManager()->IsKeyPressed(Key::F2))
 		{
 			strcpy_s(nodeName, 1024, child->GetName().c_str());
 			currentEditedNode = child;
 			startRenaming = true;
 		}

		if (nodeOpen && childHasChildren)
		{
			if (renderChildren)
			{
				RenderChildren(child);
			}
			ImGui::TreePop();			
		}		
	}
}

void HierarchyWindow::SelectedNodeChanged(NodeDescriptor *node)
{
	newSelectedNode = node;
}
