#include "CameraWindow.h"
#include "Descriptors/CameraDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"
#include "AssetUtils.h"
#include "imgui.h"

using namespace FireCube;

CameraWindow::CameraWindow(FireCube::Engine *engine) : Object(engine)
{

}

void CameraWindow::Render(EditorState *editorState, CameraDescriptor *descriptor)
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		{			
			const char *projectionTypesStr[] = { "Perspective", "Orthographic" };
			const char *selectedType;
			bool oldOrthographic = descriptor->GetOrthographic();
			if (descriptor->GetOrthographic())
			{
				selectedType = projectionTypesStr[1];
			}
			else
			{
				selectedType = projectionTypesStr[0];
			}

			if (ImGui::BeginCombo("Projection", selectedType))
			{
				for (int i = 0; i < IM_ARRAYSIZE(projectionTypesStr); i++)
				{
					bool isSelected = i == 0 ? !descriptor->GetOrthographic() : descriptor->GetOrthographic();
					if (ImGui::Selectable(projectionTypesStr[i], isSelected))
					{
						bool newOrthographic = i == 1;						
						
						auto command = new CustomCommand(editorState, "Change Projection Type", [descriptor, newOrthographic]()
						{
							descriptor->SetOrthographic(newOrthographic);
							descriptor->componentChanged(nullptr);
						}, [descriptor, oldOrthographic]()
						{
							descriptor->SetOrthographic(oldOrthographic);
							descriptor->componentChanged(nullptr);
						});

						editorState->ExecuteCommand(command);
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		if (descriptor->GetOrthographic())
		{
			zNearInput.Render("Z Near", editorState, "Change Near Plane", [descriptor]() {
				return descriptor->GetNearPlane();
			}, [descriptor](float newValue) {
				descriptor->SetNearPlane(newValue);
			});

			zFarInput.Render("Z Far", editorState, "Change Far Plane", [descriptor]() {
				return descriptor->GetFarPlane();
			}, [descriptor](float newValue) {
				descriptor->SetFarPlane(newValue);
			});

			leftInput.Render("Left", editorState, "Change Left Plane", [descriptor]() {
				return descriptor->GetLeftPlane();
			}, [descriptor](float newValue) {
				descriptor->SetLeftPlane(newValue);
			});

			rightInput.Render("Right", editorState, "Change Right Plane", [descriptor]() {
				return descriptor->GetRightPlane();
			}, [descriptor](float newValue) {
				descriptor->SetRightPlane(newValue);
			});

			bottomInput.Render("Bottom", editorState, "Change Bottom Plane", [descriptor]() {
				return descriptor->GetBottomPlane();
			}, [descriptor](float newValue) {
				descriptor->SetBottomPlane(newValue);
			});

			topInput.Render("Top", editorState, "Change Top Plane", [descriptor]() {
				return descriptor->GetTopPlane();
			}, [descriptor](float newValue) {
				descriptor->SetTopPlane(newValue);
			});
		}
		else
		{
			zNearInput.Render("Z Near", editorState, "Change Near Plane", [descriptor]() {
				return descriptor->GetNearPlane();
			}, [descriptor](float newValue) {
				descriptor->SetNearPlane(newValue);
			});

			zFarInput.Render("Z Far", editorState, "Change Far Plane", [descriptor]() {
				return descriptor->GetFarPlane();
			}, [descriptor](float newValue) {
				descriptor->SetFarPlane(newValue);
			});

			fovInput.Render("FOV", editorState, "Change Fov", [descriptor]() {
				return descriptor->GetFOV();
			}, [descriptor](float newValue) {
				descriptor->SetFOV(newValue);
			});
		}
	}
}

