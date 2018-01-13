#include "LightWindow.h"
#include "Descriptors/LightDescriptor.h"
#include "Commands/CustomCommand.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"

using namespace FireCube;

LightWindow::LightWindow(Engine *engine) : Object(engine)
{

}

void LightWindow::Render(EditorState *editorState, LightDescriptor *descriptor)
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const LightType lightTypes[] = { LightType::DIRECTIONAL, LightType::POINT, LightType::SPOT };
		const char *lightTypesStr[] = { "Directional", "Point", "Spot"};
		const char *selectedType;
		LightType oldLightType = descriptor->GetLightType();
		if (descriptor->GetLightType() == LightType::DIRECTIONAL)
		{
			selectedType = lightTypesStr[0];
		}
		else if (descriptor->GetLightType() == LightType::POINT)
		{
			selectedType = lightTypesStr[1];
		}
		else if (descriptor->GetLightType() == LightType::SPOT)
		{
			selectedType = lightTypesStr[2];
		}
		if (ImGui::BeginCombo("Type", selectedType))
		{
			for (int i = 0; i < IM_ARRAYSIZE(lightTypesStr); i++)
			{
				bool isSelected = (descriptor->GetLightType() == lightTypes[i]);
				if (ImGui::Selectable(lightTypesStr[i], isSelected))
				{
					LightType newLightType = lightTypes[i];
					auto command = new CustomCommand(editorState, "Change Light Type", [descriptor, newLightType]()
					{
						descriptor->SetLightType(newLightType);
						descriptor->componentChanged(nullptr);
					}, [descriptor, oldLightType]()
					{
						descriptor->SetLightType(oldLightType);
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

		float color[3] = { descriptor->GetColor().x, descriptor->GetColor().y, descriptor->GetColor().z };
		ImGui::ColorEdit3("Color", color);
		descriptor->SetColor(vec3(color[0], color[1], color[2]));
	}
}
