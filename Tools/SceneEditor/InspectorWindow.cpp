#include "InspectorWindow.h"
#include "imgui.h"
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"
#include "Commands/TransformCommands.h"

using namespace FireCube;

InspectorWindow::InspectorWindow(Engine *engine) : Object(engine), isActive(false)
{

}

void InspectorWindow::Render()
{
	NodeDescriptor *selectedNode = editorState->GetSelectedNode();

	ImGui::SetNextDock(ImGuiDockSlot_Right);
	if (ImGui::BeginDock("Inspector", nullptr, 0, ImVec2(50, -1)))
	{
		if (selectedNode && ImGui::CollapsingHeader("Node", ImGuiTreeNodeFlags_DefaultOpen))
		{
			translationInput.Render();
			rotationInput.Render();
			scaleInput.Render();
		}
	}
	ImGui::EndDock();
}

void InspectorWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;

	translationInput.Init("Position", editorState, [this]() {
		return this->editorState->GetSelectedNode()->GetTranslation();
	}, [this](vec3 newValue, vec3 prevValue) -> Command * {
		return new SetTranslationCommand(this->editorState, "Translate", this->editorState->GetSelectedNode(), prevValue, newValue);
	});

	rotationInput.Init("Rotation", editorState, [this]() {
		vec3 rotationAngles = this->editorState->GetSelectedNode()->GetNode()->GetRotation().GetMatrix().ExtractEulerAngles();
		return vec3(rotationAngles.x / PI * 180.0f, rotationAngles.y / PI * 180.0f, rotationAngles.z / PI * 180.0f);
	}, [this](vec3 newValue, vec3 prevValue) -> Command * {
		return new SetRotationCommand(this->editorState, "Rotate", this->editorState->GetSelectedNode(), prevValue / 180.0f * PI, newValue / 180.0f * PI);
	});

	scaleInput.Init("Scale", editorState, [this]() {
		return this->editorState->GetSelectedNode()->GetScale();
	}, [this](vec3 newValue, vec3 prevValue) -> Command * {
		return new SetScaleCommand(this->editorState, "Scale", this->editorState->GetSelectedNode(), prevValue, newValue);
	});
}
