#include "MaterialEditorWindow.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"
#include "ImguiHelpers.h"

using namespace FireCube;

MaterialEditorWindow::MaterialEditorWindow(Engine *engine) : Object(engine), auxRenderWindow(engine), isOpen(false)
{

}

void MaterialEditorWindow::Render()
{	
	bool prevIsOpen = isOpen;

	if (!isOpen)
	{
		return;
	}

	if (ImGui::Begin("Material Editor", &isOpen))
	{
		if (ImGui::Button("New"))
		{
			material = new Material(engine);
			material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
			material->SetParameter(PARAM_MATERIAL_SPECULAR, vec3(0.0f));
			material->SetParameter(PARAM_MATERIAL_SHININESS, 0.0f);
			material->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
			material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
			currentFileName = "";
			UpdatePreview(material);
		}

		ImGui::SameLine();
		{
			if (ImGui::Button("Open"))
			{
				char chosenPath[1024];
				OPENFILENAMEA ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = chosenPath;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(chosenPath);
				ofn.lpstrFilter = "Material\0*.xml\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetOpenFileNameA(&ofn) != 0)
				{
					std::string filename = chosenPath;
					if (filename.empty() == false)
					{
						std::replace(filename.begin(), filename.end(), '/', '\\');

						if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), filename))
						{
							currentFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), filename);
							std::replace(currentFileName.begin(), currentFileName.end(), '\\', '/');
							material = engine->GetResourceCache()->GetResource<Material>(currentFileName);
							UpdatePreview(material);
						}
						else
						{

						}
					}
				}
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			if (currentFileName.empty())
			{			
				char chosenPath[1024];				
				OPENFILENAMEA ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = chosenPath;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(chosenPath);
				ofn.lpstrFilter = "Material\0*.xml\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;				
				if (GetSaveFileNameA(&ofn) != 0)
				{
					std::string path = chosenPath;
					if (path.empty() == false)
					{
						std::replace(path.begin(), path.end(), '/', '\\');
						if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), path))
						{
							currentFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);
							std::replace(currentFileName.begin(), currentFileName.end(), '\\', '/');
							AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
						}
					}
				}				
			}
			else
			{
				AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
			}
		}		

		ImGui::SameLine();

		if (ImGui::Button("Save As"))
		{
			char chosenPath[1024];
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = chosenPath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(chosenPath);
			ofn.lpstrFilter = "Material\0*.xml\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			if (GetSaveFileNameA(&ofn) != 0)
			{
				std::string path = chosenPath;
				if (path.empty() == false)
				{
					std::replace(path.begin(), path.end(), '/', '\\');
					if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), path))
					{
						currentFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);
						std::replace(currentFileName.begin(), currentFileName.end(), '\\', '/');
						AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
					}
				}
			}						
		}

		ImGui::SameLine();
		if (ImGui::Button("Pick"))
		{
			editorState->startMaterialPick(editorState);
		}

		ImGui::SameLine();
		if (ImGui::Button("Add Parameter"))
		{
			ImGui::OpenPopup("Select Type");
		}

		{
			static MaterialParameterType types[] = { MaterialParameterType::FLOAT, MaterialParameterType::INT, MaterialParameterType::BOOL, 
													 MaterialParameterType::VEC2, MaterialParameterType::VEC3, MaterialParameterType::VEC4, 
													 MaterialParameterType::RGB, MaterialParameterType::RGBA };
			static char *typesStr[] = { "Float", "Int", "Bool", "Vec2", "Vec3", "Vec4", "RGB", "RGBA" };
			
			static MaterialParameterType selectedType;
			static char parameterName[256] = "";
			bool selected = false;
			if (ImGui::BeginPopup("Select Type"))
			{				
				for (int i = 0; i < IM_ARRAYSIZE(types); i++)
				{
					if (ImGui::Selectable(typesStr[i]))
					{
						selectedType = types[i];
						selected = true;
					}
				}
				ImGui::EndPopup();
			}
			
			if (selected)
			{
				selected = false;
				parameterName[0] = 0;
				ImGui::OpenPopup("Add Parameter");
			}
			
			if (ImGui::BeginPopupModal("Add Parameter", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive())
				{
					ImGui::SetKeyboardFocusHere();
				}
				bool textAccepted = ImGui::InputText("Parameter Name", parameterName, 256, ImGuiInputTextFlags_EnterReturnsTrue);
				if (ImGui::Button("OK", ImVec2(120, 0)) || textAccepted)
				{
					if (selectedType == MaterialParameterType::FLOAT)
					{
						material->SetParameter(parameterName, 0.0f);						
					}
					else if (selectedType == MaterialParameterType::INT)
					{
						material->SetParameter(parameterName, 0);
					}
					else if (selectedType == MaterialParameterType::BOOL)
					{
						material->SetParameter(parameterName, false);
					}
					else if (selectedType == MaterialParameterType::VEC2)
					{
						material->SetParameter(parameterName, vec2(0.0f));
					}
					else if (selectedType == MaterialParameterType::VEC3)
					{
						material->SetParameter(parameterName, vec3(0.0f));
					}
					else if (selectedType == MaterialParameterType::VEC4)
					{
						material->SetParameter(parameterName, vec4(0.0f));
					}
					else if (selectedType == MaterialParameterType::RGB)
					{
						material->SetRGBParameter(parameterName, vec3(0.0f));
					}
					else if (selectedType == MaterialParameterType::RGBA)
					{
						material->SetRGBAParameter(parameterName, vec4(0.0f));
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		if (material)
		{
			char name[256];
			strcpy_s(name, 256, material->GetName().c_str());
			if (ImGui::InputText("Name", name, 256))
			{
				material->SetName(name);
			}

			std::map<StringHash, MaterialParameter> paramters = material->GetParameters();

			for (auto &p : paramters)
			{
				MaterialParameter &paramter = p.second;
				std::string &parameterName = paramter.name;
				StringHash nameHash(parameterName);
				MaterialParameterType type = p.second.type;
				
				if (type == MaterialParameterType::NONE)
				{
					switch (material->GetParameter(nameHash).value.GetType())
					{
					case VariantType::FLOAT:
						type = MaterialParameterType::FLOAT;
						break;
					case VariantType::VEC2:
						type = MaterialParameterType::VEC2;
						break;
					case VariantType::VEC3:
						type = MaterialParameterType::VEC3;
						break;
					case VariantType::VEC4:
						type = MaterialParameterType::VEC4;
						break;
					case VariantType::INT:
						type = MaterialParameterType::INT;
						break;
					case VariantType::BOOL:
						type = MaterialParameterType::BOOL;
						break;
					default:
						break;
					}
				}
			
				if (type == MaterialParameterType::RGB)
				{
					vec3 value = material->GetParameter(nameHash).value.GetVec3();
					if (ImGui::ColorEdit3(parameterName.c_str(), &value.x))
					{
						material->SetRGBParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::RGBA)
				{
					vec4 value = material->GetParameter(nameHash).value.GetVec4();
					if (ImGui::ColorEdit4(parameterName.c_str(), &value.x))
					{
						material->SetRGBAParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::FLOAT)
				{
					float value = material->GetParameter(nameHash).value.GetFloat();
					if (ImGui::InputFloat(parameterName.c_str(), &value))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::VEC2)
				{
					vec2 value = material->GetParameter(nameHash).value.GetVec2();
					if (ImGui::InputFloat2(parameterName.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::VEC3)
				{
					vec3 value = material->GetParameter(nameHash).value.GetVec3();
					if (ImGui::InputFloat3(parameterName.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::VEC4)
				{
					vec4 value = material->GetParameter(nameHash).value.GetVec4();
					if (ImGui::InputFloat4(parameterName.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::INT)
				{
					int value = material->GetParameter(nameHash).value.GetInt();
					if (ImGui::InputInt(parameterName.c_str(), &value))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == MaterialParameterType::BOOL)
				{
					bool value = material->GetParameter(nameHash).value.GetBool();
					if (ImGui::Checkbox(parameterName.c_str(), &value))
					{
						material->SetParameter(parameterName, value);
					}
				}

				ImGui::PushID(parameterName.c_str());
				if (ImGui::BeginPopupContextItem("item context menu"))
				{
					if (ImGui::Selectable("Remove"))
					{
						material->RemoveParameter(parameterName);
					}
					
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}

			const TextureUnit textureTypes[] = { TextureUnit::DIFFUSE, TextureUnit::NORMAL, TextureUnit::SPECULAR };
			const std::string textureTypesLabels[] = { "Diffuse texture" , "Normal texture", "Specular texture" };

			for (int i = 0; i < 3; ++i)
			{
				std::string selectedPath;
				TextureUnit textureUnit = textureTypes[i];
				std::string label = textureTypesLabels[i];

				ImGui::BeginGroup();
				ImGui::PushID(i);
				std::string filename = material->GetTexture(textureUnit) ? material->GetTexture(textureUnit)->GetFileName() : "";
				ImGui::InputText("", &filename[0], filename.size() + 1, ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					ImGuiHelpers::ShowAssetSelectionPopup("Select Texture");
				}
				ImGui::SameLine();
				ImGui::Text(label.c_str());
				if (ImGuiHelpers::AssetSelectionPopup("Select Texture", AssetType::TEXTURE, selectedPath))
				{
					material->SetTexture(textureUnit, engine->GetResourceCache()->GetResource<Texture2D>(selectedPath));
				}
				ImGui::PopID();
				ImGui::EndGroup();
				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("me_asset");
					if (payload)
					{
						AssetType type;
						std::string path;
						AssetUtils::DeserializeAssetDescription((const char *)payload->Data, type, path);
						std::replace(path.begin(), path.end(), '\\', '/');
						path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

						if (type == AssetType::TEXTURE)
						{
							material->SetTexture(textureUnit, engine->GetResourceCache()->GetResource<Texture2D>(path));
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			{
				std::string selectedPath;
				ImGui::BeginGroup();
				ImGui::PushID("technique");
				std::string filename = material->GetTechnique() ? material->GetTechnique()->GetFileName() : "";
				ImGui::InputText("", &filename[0], filename.size() + 1, ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					ImGuiHelpers::ShowAssetSelectionPopup("Select Technique");
				}
				ImGui::SameLine();
				ImGui::Text("Technique");
				if (ImGuiHelpers::AssetSelectionPopup("Select Technique", AssetType::TECHNIQUE, selectedPath))
				{
					material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>(selectedPath));
				}
				ImGui::PopID();
				ImGui::EndGroup();
				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("me_asset");
					if (payload)
					{
						AssetType type;
						std::string path;
						AssetUtils::DeserializeAssetDescription((const char *)payload->Data, type, path);
						std::replace(path.begin(), path.end(), '\\', '/');
						path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

						if (type == AssetType::TECHNIQUE)
						{
							material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>(path));
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}

		auxRenderWindow.Render();
	}
	ImGui::End();

	if (prevIsOpen && !isOpen)
	{
		// Closing window
		if (!currentFileName.empty())
		{
			AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
		}
	}
}

void MaterialEditorWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;

	SubscribeToEvent(editorState->materialPicked, &MaterialEditorWindow::MaterialPicked);
}

bool MaterialEditorWindow::IsOpen() const
{
	return isOpen;
}

void MaterialEditorWindow::UpdatePreview(FireCube::Material *material)
{
	auxRenderWindow.Reset();
	auto root = auxRenderWindow.GetRoot();

	auto lightNode1 = root->CreateChild();
	auto light = lightNode1->CreateComponent<Light>();
	light->SetLightType(LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));
	lightNode1->Rotate(vec3((float)-PI * 0.25f, (float)-PI * 0.25f, 0.0f));

	auto lightNode2 = root->CreateChild();
	light = lightNode2->CreateComponent<Light>();
	light->SetLightType(LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));
	lightNode2->Rotate(vec3((float)PI * 0.25f, (float)PI * 0.25f, 0.0f));

	auto meshNode = root->CreateChild();	
	auto geometry = GeometryGenerator::GenerateSphere(engine, 0.5f, 16, 16);
	SharedPtr<Mesh> sphereMesh = new Mesh(engine);
	sphereMesh->AddGeometry(geometry, BoundingBox(vec3(-0.5f), vec3(0.5f)), material);
	meshNode->CreateComponent<StaticModel>(sphereMesh);
}

bool *MaterialEditorWindow::GetIsOpenPtr()
{
	return &isOpen;
}

void MaterialEditorWindow::Show()
{
	isOpen = true;
}

void MaterialEditorWindow::MaterialPicked(Material *material)
{
	isOpen = true;
	this->material = material;
	currentFileName = material->GetFileName();	
	UpdatePreview(material);
}
