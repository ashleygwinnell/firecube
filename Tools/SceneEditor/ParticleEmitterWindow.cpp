#include "ParticleEmitterWindow.h"
#include "Descriptors/ParticleEmitterDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"
#include "AssetUtils.h"
#include "imgui_internal.h"

using namespace FireCube;

ParticleEmitterWindow::ParticleEmitterWindow(FireCube::Engine *engine) : Object(engine)
{

}

void ParticleEmitterWindow::Render(EditorState *editorState, ParticleEmitterDescriptor *descriptor)
{
	{
		const ParticleEmitterShape emitterTypes[] = { ParticleEmitterShape::BOX, ParticleEmitterShape::SPHERE };
		const char *emitterTypesStr[] = { "Box", "Sphere" };
		const char *selectedType;
		ParticleEmitterShape oldEmitterType = descriptor->GetEmitterShape();
		if (descriptor->GetEmitterShape() == ParticleEmitterShape::BOX)
		{
			selectedType = emitterTypesStr[0];
		}
		else if (descriptor->GetEmitterShape() == ParticleEmitterShape::SPHERE)
		{
			selectedType = emitterTypesStr[1];
		}

		if (ImGui::BeginCombo("Type", selectedType))
		{
			for (int i = 0; i < IM_ARRAYSIZE(emitterTypesStr); i++)
			{
				bool isSelected = (descriptor->GetEmitterShape() == emitterTypes[i]);
				if (ImGui::Selectable(emitterTypesStr[i], isSelected))
				{
					ParticleEmitterShape newShape = emitterTypes[i];
					ParticleEmitterShape oldShape = descriptor->GetEmitterShape();
					vec3 oldBox = descriptor->GetBox();
					float oldRadius = descriptor->GetRadius();

					auto command = new CustomCommand(editorState, "Change Shape Type", [descriptor, newShape, oldBox, oldRadius]()
					{
						switch (newShape)
						{
						case ParticleEmitterShape::BOX:
							descriptor->SetBoxEmitter(oldBox);
							break;
						case ParticleEmitterShape::SPHERE:
							descriptor->SetSphereEmitter(oldRadius);
							break;
						default:
							break;
						}
						descriptor->componentChanged(nullptr);
					}, [descriptor, oldShape, oldBox, oldRadius]()
					{
						switch (oldShape)
						{
						case ParticleEmitterShape::BOX:
							descriptor->SetBoxEmitter(oldBox);
							break;
						case ParticleEmitterShape::SPHERE:
							descriptor->SetSphereEmitter(oldRadius);
							break;
						default:
							break;
						}
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

	if (descriptor->GetEmitterShape() == ParticleEmitterShape::BOX)
	{
		sizeInput.Render("Size", editorState, "Change Box", [descriptor]() {
			return descriptor->GetBox();
		}, [descriptor](vec3 newValue) {
			descriptor->SetBoxEmitter(newValue);
		});
	}
	else if (descriptor->GetEmitterShape() == ParticleEmitterShape::SPHERE)
	{
		radiusInput.Render("Radius", editorState, "Change Radius", [descriptor]() {
			return descriptor->GetRadius();
		}, [descriptor](float newValue) {
			descriptor->SetSphereEmitter(newValue);
		});
	}

	particleCountInput.Render("Particle Count", editorState, "Change Number Of Particles", [descriptor]() {
		return descriptor->GetNumberOfParticles();
	}, [descriptor](unsigned int newValue) {
		descriptor->SetNumberOfParticles(newValue);
	});

	emissionRateInput.Render("Emission Rate", editorState, "Change Emission Rate", [descriptor]() {
		return descriptor->GetEmissionRate();
	}, [descriptor](unsigned int newValue) {
		descriptor->SetEmissionRate(newValue);
	});

	prewarmCheckBox.Render("Prewarm", editorState, "Change Prewarm", [descriptor]() {
		return descriptor->GetPrewarm();
	}, [descriptor](bool value) {
		descriptor->SetPrewarm(value);
	});

	lifeTimeInput.Render("Life Time", editorState, "Change Life Time", [descriptor]() {
		return vec2(descriptor->GetMinLifeTime(), descriptor->GetMaxLifeTime());
	}, [descriptor](vec2 value) {
		descriptor->SetMinLifeTime(value.x);
		descriptor->SetMaxLifeTime(value.y);
	});

	speedInput.Render("Speed", editorState, "Change Speed", [descriptor]() {
		return vec2(descriptor->GetMinSpeed(), descriptor->GetMaxSpeed());
	}, [descriptor](vec2 value) {
		descriptor->SetMinSpeed(value.x);
		descriptor->SetMaxSpeed(value.y);
	});

	{
		std::string selectedPath;
		std::string materialFileName = descriptor->GetMaterial();
		ImGui::InputText("", &materialFileName[0], materialFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			ImGuiHelpers::ShowAssetSelectionPopup("Select Material");
		}
		ImGui::SameLine();
		ImGui::Text("Material");
		if (ImGuiHelpers::AssetSelectionPopup("Select Material", AssetType::MATERIAL, selectedPath))
		{
			std::string oldValue = descriptor->GetMaterial();

			auto command = new CustomCommand(editorState, "Change Material", [descriptor, selectedPath, this]()
			{
				descriptor->SetMaterial(selectedPath, engine);
				descriptor->componentChanged(nullptr);
			}, [descriptor, oldValue, this]()
			{
				descriptor->SetMaterial(oldValue, engine);
				descriptor->componentChanged(nullptr);
			});

			editorState->ExecuteCommand(command);
		}
	}

	{
		const ParticleEmitterSimulationSpace simulationSpaceTypes[] = { ParticleEmitterSimulationSpace::LOCAL, ParticleEmitterSimulationSpace::WORLD };
		const char *simulationSpaceTypesStr[] = { "Local", "World" };
		const char *selectedType;
		ParticleEmitterSimulationSpace oldSimulationSpace = descriptor->GetSimulationSpace();
		if (descriptor->GetSimulationSpace() == ParticleEmitterSimulationSpace::LOCAL)
		{
			selectedType = simulationSpaceTypesStr[0];
		}
		else if (descriptor->GetSimulationSpace() == ParticleEmitterSimulationSpace::WORLD)
		{
			selectedType = simulationSpaceTypesStr[1];
		}

		if (ImGui::BeginCombo("Simulation Space", selectedType))
		{
			for (int i = 0; i < IM_ARRAYSIZE(simulationSpaceTypesStr); i++)
			{
				bool isSelected = (descriptor->GetSimulationSpace() == simulationSpaceTypes[i]);
				if (ImGui::Selectable(simulationSpaceTypesStr[i], isSelected))
				{
					ParticleEmitterSimulationSpace newSimulationSpace = simulationSpaceTypes[i];

					auto command = new CustomCommand(editorState, "Change Simulation Space", [descriptor, newSimulationSpace]()
					{
						descriptor->SetSimulationSpace(newSimulationSpace);
						descriptor->componentChanged(nullptr);
					}, [descriptor, oldSimulationSpace]()
					{
						descriptor->SetSimulationSpace(oldSimulationSpace);
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

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha *  (descriptor->IsEnabled() ? 0.5f : 1.0f));
	if (ImGui::ButtonEx("Play", ImVec2(0, 0), descriptor->IsEnabled() ? ImGuiButtonFlags_Disabled : 0))
	{
		descriptor->SetEnabled(true);
	}
	ImGui::PopStyleVar();
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha *  (descriptor->IsEnabled() ? 1.0f : 0.5f));
	if (ImGui::ButtonEx("Stop", ImVec2(0, 0), descriptor->IsEnabled() ? 0 : ImGuiButtonFlags_Disabled))
	{
		descriptor->SetEnabled(false);
	}
	ImGui::PopStyleVar();
}

