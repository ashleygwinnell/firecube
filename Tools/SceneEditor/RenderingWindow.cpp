#include "imgui.h"
#include "RenderingWindow.h"

using namespace FireCube;

RenderingWindow::RenderingWindow(Engine *engine) : Object(engine), isOpen(true)
{

}

void RenderingWindow::Render()
{
	if (!isOpen)
	{
		return;
	}

	if (ImGui::Begin("Rendering", &isOpen))
	{
		fogColorInput.Render("Fog Color", editorState, "Change Fog Color", [this]() {
			return scene->GetFogColor();
		}, [this](vec3 color) {
			scene->SetFogColor(color);
		});
	}
	ImGui::End();
}

void RenderingWindow::SetScene(Scene *scene, NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->scene = scene;
	this->rootDesc = rootDesc;
	this->editorState = editorState;
}

bool *RenderingWindow::GetIsOpenPtr()
{
	return &isOpen;
}
