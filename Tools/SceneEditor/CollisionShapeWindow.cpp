#include "CollisionShapeWindow.h"
#include "Descriptors/CollisionShapeDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"

using namespace FireCube;

CollisionShapeWindow::CollisionShapeWindow(FireCube::Engine *engine) : Object(engine)
{

}

void CollisionShapeWindow::Render(EditorState *editorState, CollisionShapeDescriptor *descriptor)
{
	const CollisionShapeType shapeTypes[] = { CollisionShapeType::BOX, CollisionShapeType::PLANE, CollisionShapeType::TRIANGLE_MESH, CollisionShapeType::SPHERE };
	const char *shapeTypesStr[] = { "Box", "Plane", "Mesh", "Sphere" };
	const char *selectedType;
	CollisionShapeType oldShapeType = descriptor->GetShapeType();
	if (oldShapeType == CollisionShapeType::BOX)
	{
		selectedType = shapeTypesStr[0];
	}
	else if (oldShapeType == CollisionShapeType::PLANE)
	{
		selectedType = shapeTypesStr[1];
	}
	else if (oldShapeType == CollisionShapeType::TRIANGLE_MESH)
	{
		selectedType = shapeTypesStr[2];
	}
	else if (oldShapeType == CollisionShapeType::SPHERE)
	{
		selectedType = shapeTypesStr[3];
	}
	if (ImGui::BeginCombo("Type", selectedType))
	{
		std::string oldMesh = descriptor->GetMeshFilename();
		Plane oldPlane = descriptor->GetPlane();
		BoundingBox oldBoundingBox = descriptor->GetBox();
		float oldRadius = descriptor->GetRadius();

		for (int i = 0; i < IM_ARRAYSIZE(shapeTypesStr); i++)
		{
			bool isSelected = (descriptor->GetShapeType() == shapeTypes[i]);
			if (ImGui::Selectable(shapeTypesStr[i], isSelected))
			{
				CollisionShapeType newShapeType = shapeTypes[i];
				auto command = new CustomCommand(editorState, "Change Shape Type", [descriptor, newShapeType, oldPlane, oldBoundingBox, oldMesh, oldRadius, this]()
				{
					switch (newShapeType)
					{
					case CollisionShapeType::TRIANGLE_MESH:
						descriptor->SetMesh(oldMesh, engine);
						break;
					case CollisionShapeType::PLANE:
						descriptor->SetPlane(oldPlane);
						break;
					case CollisionShapeType::BOX:
						descriptor->SetBox(oldBoundingBox);
						break;
					case CollisionShapeType::SPHERE:
						descriptor->SetSphere(oldRadius);
						break;
					default:
						break;
					}
					descriptor->componentChanged(nullptr);
				}, [descriptor, oldShapeType, oldPlane, oldBoundingBox, oldMesh, oldRadius, this]()
				{
					switch (oldShapeType)
					{
					case CollisionShapeType::TRIANGLE_MESH:
						descriptor->SetMesh(oldMesh, engine);
						break;
					case CollisionShapeType::PLANE:
						descriptor->SetPlane(oldPlane);
						break;
					case CollisionShapeType::BOX:
						descriptor->SetBox(oldBoundingBox);
						break;
					case CollisionShapeType::SPHERE:
						descriptor->SetSphere(oldRadius);
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

	triggerCheckBox.Render("Trigger", editorState, "Change Trigger", [descriptor]() {
		return descriptor->IsTrigger();
	}, [descriptor](bool newValue) {
		descriptor->SetIsTrigger(newValue);
	});

	if (descriptor->GetShapeType() == CollisionShapeType::BOX)
	{
		boxMinInput.Render("Min", editorState, "Change Box", [descriptor]() {
			return descriptor->GetBox().GetMin();
		}, [descriptor](vec3 newValue) {
			descriptor->SetBox(BoundingBox(newValue, descriptor->GetBox().GetMax()));
		});
		boxMaxInput.Render("Max", editorState, "Change Box", [descriptor]() {
			return descriptor->GetBox().GetMax();
		}, [descriptor](vec3 newValue) {
			descriptor->SetBox(BoundingBox(descriptor->GetBox().GetMin(), newValue));
		});
	}
	else if (descriptor->GetShapeType() == CollisionShapeType::PLANE)
	{
		planeInput.Render("Plane", editorState, "Change Plane", [descriptor]() {
			return vec4(descriptor->GetPlane().GetNormal(), descriptor->GetPlane().GetDistance());
		}, [descriptor](vec4 newValue) {
			Plane plane(vec3(newValue.x, newValue.y, newValue.z), newValue.w);
			descriptor->SetPlane(plane);
		});
	}
	else if (descriptor->GetShapeType() == CollisionShapeType::TRIANGLE_MESH)
	{
		std::string selectedPath;
		std::string meshFileName = descriptor->GetMeshFilename();
		ImGui::InputText("", &meshFileName[0], meshFileName.size() + 1, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			ImGuiHelpers::ShowAssetSelectionPopup("Select Mesh");
		}
		ImGui::SameLine();
		ImGui::Text("Mesh");
		if (ImGuiHelpers::AssetSelectionPopup("Select Mesh", AssetType::MESH, selectedPath))
		{
			std::string oldMeshFileName = descriptor->GetMeshFilename();

			auto command = new CustomCommand(editorState, "Change Mesh", [descriptor, selectedPath, this]()
			{
				descriptor->SetMesh(selectedPath, engine);
				descriptor->componentChanged(nullptr);
			}, [descriptor, oldMeshFileName, this]()
			{
				descriptor->SetMesh(oldMeshFileName, engine);
				descriptor->componentChanged(nullptr);
			});

			editorState->ExecuteCommand(command);
		}
	}
	else if (descriptor->GetShapeType() == CollisionShapeType::SPHERE)
	{
		sphereRadiusInput.Render("Radius", editorState, "Change Radius", [descriptor]() {
			return descriptor->GetRadius();
		}, [descriptor](float newValue) {
			descriptor->SetSphere(newValue);
		});
	}
}

