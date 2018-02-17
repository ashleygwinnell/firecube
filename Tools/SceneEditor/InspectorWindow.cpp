#include "InspectorWindow.h"
#include "imgui.h"
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"
#include "Commands/TransformCommands.h"
#include "Commands/RenameNodeCommand.h"
#include "Descriptors/ComponentDescriptor.h"
#include "Commands/RemoveComponentCommand.h"
#include "AssetUtils.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "Commands/AddComponentCommand.h"

using namespace FireCube;

InspectorWindow::InspectorWindow(Engine *engine) : Object(engine), staticModelWindow(engine), lightWindow(engine), luaScriptWindow(engine), collisionShapeWindow(engine),
characterControllerWindow(engine), boxWindow(engine), rigidBodyWindow(engine), planeWindow(engine), sphereWindow(engine), particleEmitterWindow(engine), cameraWindow(engine), isOpen(true)
{
	componentTypeToLabel = { 
		{ ComponentType::BOX, "Box"},
		{ ComponentType::PLANE, "Plane" },
		{ ComponentType::SPHERE, "Sphere" },
		{ ComponentType::STATIC_MODEL, "StaticModel" },
		{ ComponentType::LIGHT, "Light" },
		{ ComponentType::PARTICLE_EMITTER, "ParticleEmitter" },
		{ ComponentType::COLLISION_SHAPE, "CollisionShape" },
		{ ComponentType::LUA_SCRIPT, "LuaScript" },
		{ ComponentType::CAMERA, "Camera" },
		{ ComponentType::CHARACTER_CONTROLLER, "CharacterController" },
		{ ComponentType::PHYSICS_WORLD, "PhysicsWorld" },
		{ ComponentType::RIGID_BODY, "RigidBody" },	
	};
}

void InspectorWindow::Render()
{
	NodeDescriptor *selectedNode = editorState->GetSelectedNode();

	ImGui::SetNextDock(ImGuiDockSlot_Right);
	if (ImGui::BeginDock("Inspector", &isOpen))
	{
		if (selectedNode && ImGui::CollapsingHeader("Node", ImGuiTreeNodeFlags_DefaultOpen))
		{
			char name[1024];
			std::string nameStr = selectedNode->GetName();
			std::copy(nameStr.begin(), nameStr.end(), name);
			name[nameStr.size()] = '\0';
			if (ImGui::InputText("Name", name, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				auto command = new RenameNodeCommand(editorState, "Rename", selectedNode, name);
				editorState->ExecuteCommand(command);
			}

			translationInput.Render("Position", editorState, [this]() {
				return this->editorState->GetSelectedNode()->GetTranslation();
			}, [this](vec3 newValue, vec3 prevValue) -> Command * {
				return new SetTranslationCommand(this->editorState, "Translate", this->editorState->GetSelectedNode(), prevValue, newValue);
			});
			rotationInput.Render("Rotation", editorState, [this]() {
				vec3 rotationAngles = this->editorState->GetSelectedNode()->GetNode()->GetRotation().GetMatrix().ExtractEulerAngles();
				return vec3(rotationAngles.x / PI * 180.0f, rotationAngles.y / PI * 180.0f, rotationAngles.z / PI * 180.0f);
			}, [this](vec3 newValue, vec3 prevValue) -> Command * {
				return new SetRotationCommand(this->editorState, "Rotate", this->editorState->GetSelectedNode(), prevValue / 180.0f * PI, newValue / 180.0f * PI);
			});
			scaleInput.Render("Scale", editorState, [this]() {
				return this->editorState->GetSelectedNode()->GetScale();
			}, [this](vec3 newValue, vec3 prevValue) -> Command * {
				return new SetScaleCommand(this->editorState, "Scale", this->editorState->GetSelectedNode(), prevValue, newValue);
			});
		}

		if (selectedNode)
		{
			auto components = selectedNode->GetComponents();
			for (auto component : components)
			{
				bool componentOpen = true;
				bool isOpen = true;
				if (ImGui::CollapsingHeader(componentTypeToLabel[component->GetType()].c_str(), &componentOpen, ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (component->GetType() == ComponentType::STATIC_MODEL)
					{
						staticModelWindow.Render(editorState, (StaticModelDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::LIGHT)
					{
						lightWindow.Render(editorState, (LightDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::LUA_SCRIPT)
					{
						luaScriptWindow.Render(editorState, (LuaScriptDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::COLLISION_SHAPE)
					{
						collisionShapeWindow.Render(editorState, (CollisionShapeDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::CHARACTER_CONTROLLER)
					{
						characterControllerWindow.Render(editorState, (CharacterControllerDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::BOX)
					{
						boxWindow.Render(editorState, (BoxDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::RIGID_BODY)
					{
						rigidBodyWindow.Render(editorState, (RigidBodyDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::PLANE)
					{
						planeWindow.Render(editorState, (PlaneDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::SPHERE)
					{
						sphereWindow.Render(editorState, (SphereDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::PARTICLE_EMITTER)
					{
						particleEmitterWindow.Render(editorState, (ParticleEmitterDescriptor *)component);
					}
					else if (component->GetType() == ComponentType::CAMERA)
					{
						cameraWindow.Render(editorState, (CameraDescriptor *)component);
					}
				}

				if (componentOpen == false)
				{
					auto removeComponentCommand = new RemoveComponentCommand(editorState, "Remove Component", component->GetParent(), component, engine);
					editorState->ExecuteCommand(removeComponentCommand);
				}
			}
		}
	}
	ImGui::EndDock();
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset");
		if (payload)
		{
			AssetType type;
			std::string path;
			AssetUtils::DeserializeAssetDescription((const char *)payload->Data, type, path);
			std::replace(path.begin(), path.end(), '\\', '/');
			path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

			if (type == AssetType::MESH)
			{
				auto staticModelDescriptor = new StaticModelDescriptor();
				staticModelDescriptor->SetMeshFilename(path, engine);
				auto addComponentCommand = new AddComponentCommand(editorState, "Add StaticModel", selectedNode, staticModelDescriptor, engine);
				editorState->ExecuteCommand(addComponentCommand);
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void InspectorWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;	
}

bool *InspectorWindow::GetIsOpenPtr()
{
	return &isOpen;
}
