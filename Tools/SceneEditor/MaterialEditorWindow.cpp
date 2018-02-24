#include "MaterialEditorWindow.h"
#include "imgui.h"
#include "AssetUtils.h"
#include "EditorState.h"
#include "ImguiHelpers.h"

using namespace FireCube;

MaterialEditorWindow::MaterialEditorWindow(Engine *engine) : Object(engine), auxRenderWindow(engine), isOpen(false)
{
	standardParametersProperties = {
		{ PARAM_MATERIAL_DIFFUSE, std::make_pair("Diffuse", PropertyType2::COLOR) },
		{ PARAM_MATERIAL_SPECULAR, std::make_pair("Specular", PropertyType2::COLOR) },
		{ PARAM_MATERIAL_SHININESS, std::make_pair("Shininess", PropertyType2::FLOAT) },
		{ PARAM_MATERIAL_OPACITY, std::make_pair("Opacity", PropertyType2::FLOAT) },
		{ PARAM_MATERIAL_U_OFFSET, std::make_pair("U Offset", PropertyType2::VEC3) },
		{ PARAM_MATERIAL_V_OFFSET, std::make_pair("V Offset", PropertyType2::VEC3) } };
}

void MaterialEditorWindow::Render()
{	
	bool prevIsOpen = isOpen;

	if (ImGui::BeginDock("Material Editor", &isOpen))
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
			bool showFileOpen = ImGui::Button("Open");			
			const char* chosenPath = openDialog.chooseFileDialog(showFileOpen, Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Materials").c_str(), ".xml", "Select a file", ImVec2(600, 400), ImVec2(100, 100));
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

		ImGui::SameLine();
		{
			bool saveClicked = ImGui::Button("Save");
			if (currentFileName.empty())
			{				
				const char* chosenPath = saveDialog.saveFileDialog(saveClicked, nullptr, nullptr, ".xml", "Save Material file", ImVec2(600, 400), ImVec2(100, 100));
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
			else if (saveClicked)
			{
				AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
			}
		}		

		ImGui::SameLine();
		{
			bool showSaveAs = ImGui::Button("Save As");
						
			const char* chosenPath = saveDialog.saveFileDialog(showSaveAs, nullptr, nullptr, ".xml", "Save Material file", ImVec2(600, 400), ImVec2(100, 100));
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
			static PropertyType2 types[] = { PropertyType2::FLOAT, PropertyType2::VEC2, PropertyType2::VEC3, PropertyType2::VEC4 };
			static char *typesStr[] = { "Float", "Vec2", "Vec3", "Vec4" };
			
			static PropertyType2 selectedType;			
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
				ImGui::SetKeyboardFocusHere();
				bool textAccepted = ImGui::InputText("Parameter Name", parameterName, 256, ImGuiInputTextFlags_EnterReturnsTrue);
				if (ImGui::Button("OK", ImVec2(120, 0)) || textAccepted)
				{
					if (selectedType == PropertyType2::FLOAT)
					{
						material->SetParameter(parameterName, 0.0f);						
					}
					else if (selectedType == PropertyType2::VEC2)
					{
						material->SetParameter(parameterName, vec2(0.0f));
					}
					else if (selectedType == PropertyType2::VEC3)
					{
						material->SetParameter(parameterName, vec3(0.0f));
					}
					else if (selectedType == PropertyType2::VEC4)
					{
						material->SetParameter(parameterName, vec4(0.0f));
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

			std::map<StringHash, std::string> paramters = material->GetParametersNames();

			for (auto &p : paramters)
			{
				std::string parameterName = p.second;
				StringHash nameHash(parameterName);
				std::string label;
				auto standardParamProperties = standardParametersProperties.find(nameHash);
				PropertyType2 type;

				if (standardParamProperties != standardParametersProperties.end())
				{
					label = standardParamProperties->second.first;
					type = standardParamProperties->second.second;
				}
				else
				{
					label = parameterName;
					switch (material->GetParameter(nameHash).GetType())
					{
					case VariantType::FLOAT:
						type = PropertyType2::FLOAT;
						break;
					case VariantType::VEC2:
						type = PropertyType2::VEC2;
						break;
					case VariantType::VEC3:
						type = PropertyType2::VEC3;
						break;
					case VariantType::VEC4:
						type = PropertyType2::VEC4;
						break;
					default:
						break;
					}
				}
			
				if (type == PropertyType2::COLOR)
				{
					vec3 value = material->GetParameter(nameHash).GetVec3();
					if (ImGui::ColorEdit3(label.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == PropertyType2::FLOAT)
				{
					float value = material->GetParameter(nameHash).GetFloat();
					if (ImGui::InputFloat(label.c_str(), &value))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == PropertyType2::VEC2)
				{
					vec2 value = material->GetParameter(nameHash).GetVec2();
					if (ImGui::InputFloat2(label.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == PropertyType2::VEC3)
				{
					vec3 value = material->GetParameter(nameHash).GetVec3();
					if (ImGui::InputFloat3(label.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}
				else if (type == PropertyType2::VEC4)
				{
					vec4 value = material->GetParameter(nameHash).GetVec4();
					if (ImGui::InputFloat4(label.c_str(), &value.x))
					{
						material->SetParameter(parameterName, value);
					}
				}

				ImGui::PushID(parameterName.c_str());
				static float color[3];
				bool showColorPicker = false;
				if (ImGui::BeginPopupContextItem("item context menu"))
				{
					if (ImGui::Selectable("Remove"))
					{
						material->RemoveParameter(parameterName);
					}
					if (type == PropertyType2::VEC3 && ImGui::Selectable("Set From Color"))
					{
						vec3 value = material->GetParameter(nameHash).GetVec3();
						color[0] = value.x;
						color[1] = value.y;
						color[2] = value.z;
						showColorPicker = true;
					}
					ImGui::EndPopup();
				}

				if (showColorPicker)
				{
					ImGui::OpenPopup("Color Picker");
				}

				if (ImGui::BeginPopup("Color Picker"))
				{
					ImGui::ColorPicker3("##picker", (float*)&color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
					material->SetParameter(parameterName, vec3(color[0], color[1], color[2]));
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
	ImGui::EndDock();

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
	this->material = material;
	currentFileName = material->GetFileName();	
	UpdatePreview(material);
}
