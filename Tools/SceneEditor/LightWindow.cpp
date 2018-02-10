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
	const LightType lightTypes[] = { LightType::DIRECTIONAL, LightType::POINT, LightType::SPOT };
	const char *lightTypesStr[] = { "Directional", "Point", "Spot" };
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

	colorInput.Render("Color", editorState, "Change Light Color", [descriptor]() {
		return descriptor->GetColor();
	}, [descriptor](vec3 color) {
		descriptor->SetColor(color);
	});

	lightMaskInput.Render("Mask", editorState, "Change Mask", [descriptor]() {
		return descriptor->GetLightMask();
	}, [descriptor](unsigned int newValue) {
		descriptor->SetLightMask(newValue);
	});

	castShadowCheckBox.Render("Cast Shadow", editorState, "Change Cast Shadow", [descriptor]() {
		return descriptor->GetCastShadow();
	}, [descriptor](bool newValue) {
		descriptor->SetCastShadow(newValue);
	});

	shadowIntensityInput.Render("Intensity", editorState, "Change Shadow Intensity", [descriptor]() {
		return descriptor->GetShadowIntensity();
	}, [descriptor](float newValue) {
		descriptor->SetShadowIntensity(newValue);
	});

	if (descriptor->GetLightType() == LightType::POINT || descriptor->GetLightType() == LightType::SPOT)
	{
		rangeInput.Render("Range", editorState, "Change Range", [descriptor]() {
			return descriptor->GetRange();
		}, [descriptor](float newValue) {
			descriptor->SetRange(newValue);
		});
	}

	if (descriptor->GetLightType() == LightType::SPOT)
	{
		rangeInput.Render("Spot Cutoff", editorState, "Change Spot Cutoff", [descriptor]() {
			return descriptor->GetSpotCutOff() / PI * 180.0f;
		}, [descriptor](float newValue) {
			descriptor->SetSpotCutOff(newValue / 180.0f * PI);
		});
	}
}
