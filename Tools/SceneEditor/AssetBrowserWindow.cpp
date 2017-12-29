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
	}
	ImGui::EndDock();
}

void AssetBrowserWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;	
}
