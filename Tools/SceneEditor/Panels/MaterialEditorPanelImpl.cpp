#include "MaterialEditorPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../AssetUtils.h"
#include <wx/wrapsizer.h>
#include <wx/textdlg.h>
#include <wx/colordlg.h>

using namespace FireCube;

MaterialEditorPanelImpl::MaterialEditorPanelImpl(wxWindow* parent) : MaterialEditorPanel(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), editorState(((MyApp*)wxTheApp)->GetEditorState())
{
	SubscribeToEvent(editorState->materialPicked, &MaterialEditorPanelImpl::MaterialPicked);
	propertyGrid->SetDropTarget(new MaterialEditorDropTarget(this));
	
	// Replace the box sizer with a wrap sizer
	wxWrapSizer *wrapSizer = new wxWrapSizer();			
	bSizer37->Replace(bSizer38, wrapSizer);
	wrapSizer->Add(newButton, 0, wxALL, 5);	
	wrapSizer->Add(openButton, 0, wxALL, 5);	
	wrapSizer->Add(saveButton, 0, wxALL, 5);	
	wrapSizer->Add(saveAsButton, 0, wxALL, 5);	
	wrapSizer->Add(pickMaterialButton, 0, wxALL, 5);
	wrapSizer->Add(addParameterButton, 0, wxALL, 5);

	Layout();
	this->SetMinSize(wxSize(-1, -1)); // Fix bug where initially the panel has a minimum size set due to the box sizer

	standardParametersProperties = {
		{ PARAM_MATERIAL_DIFFUSE, std::make_pair("Diffuse", PropertyType::COLOR) },
		{ PARAM_MATERIAL_SPECULAR, std::make_pair("Specular", PropertyType::COLOR) },
		{ PARAM_MATERIAL_SHININESS, std::make_pair("Shininess", PropertyType::FLOAT) },
		{ PARAM_MATERIAL_OPACITY, std::make_pair("Opacity", PropertyType::FLOAT) },
		{ PARAM_U_OFFSET, std::make_pair("U Offset", PropertyType::VEC3) },
		{ PARAM_V_OFFSET, std::make_pair("V Offset", PropertyType::VEC3) } };

	propertyGrid->Bind(wxEVT_PG_RIGHT_CLICK, &MaterialEditorPanelImpl::PropertyGridRightClicked, this);
}

MaterialEditorPanelImpl::~MaterialEditorPanelImpl()
{

}

void MaterialEditorPanelImpl::NewButtonClicked(wxCommandEvent& event)
{
	material = new Material(engine);
	material->SetParameter(PARAM_MATERIAL_DIFFUSE_NAME, vec3(1.0f));
	material->SetParameter(PARAM_MATERIAL_SPECULAR_NAME, vec3(0.0f));
	material->SetParameter(PARAM_MATERIAL_SHININESS_NAME, 0.0f);
	material->SetParameter(PARAM_MATERIAL_OPACITY_NAME, 0.0f);
	currentFileName = "";
	FillPropertyGrid(material);
}

void MaterialEditorPanelImpl::OpenButtonClicked(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open", "", "", "Material files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	std::string sfile = openFileDialog.GetPath().ToStdString();

	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
	{
		currentFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);
		std::replace(currentFileName.begin(), currentFileName.end(), '\\', '/');
		material = engine->GetResourceCache()->GetResource<Material>(currentFileName);
		if (material)
		{
			FillPropertyGrid(material);
		}
	}
	else
	{
		
	}
}

void MaterialEditorPanelImpl::SaveButtonClicked(wxCommandEvent& event)
{
	if (currentFileName.empty())
	{
		SaveAsButtonClicked(event);
	}

	AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
}

void MaterialEditorPanelImpl::SaveAsButtonClicked(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, "Save Scene file", Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scenes", "", "Scene files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	std::string filename = saveFileDialog.GetPath().ToStdString();
	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), filename))
	{
		currentFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), filename);
		std::replace(currentFileName.begin(), currentFileName.end(), '\\', '/');
		AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
	}
}

void MaterialEditorPanelImpl::PickMaterialButtonClicked(wxCommandEvent& event)
{
	editorState->startMaterialPick(editorState);
}

void MaterialEditorPanelImpl::FillPropertyGrid(Material *material)
{	
	
	propertyGrid->Clear();
	
	propertyGrid->Append(new wxPropertyCategory("Material"));

	propertyGrid->Append(new wxStringProperty("Name", wxPG_LABEL, material->GetName()));	

	for (auto &p : material->GetParametersNames())
	{
		std::string parameterName = p.second;
		StringHash nameHash(parameterName);
		std::string label;
		auto standardParamProperties = standardParametersProperties.find(nameHash);
		PropertyType type;		

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
				type = PropertyType::FLOAT;
				break;
			case VariantType::VEC2:
				type = PropertyType::VEC2;
				break;
			case VariantType::VEC3:
				type = PropertyType::VEC3;
				break;
			case VariantType::VEC4:
				type = PropertyType::VEC4;
				break;
			default:				
				break;
			}
		}

		wxPGProperty *property = nullptr;
		if (type == PropertyType::COLOR)
		{
			vec3 color = material->GetParameter(nameHash).GetVec3();
			property = new wxColourProperty(label, wxPG_LABEL, wxColor(color.x * 255, color.y * 255, color.z * 255));
		}
		else if (type == PropertyType::FLOAT)
		{
			float value = material->GetParameter(nameHash).GetFloat();
			property = new wxFloatProperty(label, wxPG_LABEL, value);

		}
		else if (type == PropertyType::VEC2)
		{
			std::string str = AssetUtils::ToString(material->GetParameter(nameHash).GetVec2());
			property = new wxStringProperty(label, wxPG_LABEL, str);
		}
		else if (type == PropertyType::VEC3)
		{
			std::string str = AssetUtils::ToString(material->GetParameter(nameHash).GetVec3());
			property = new wxStringProperty(label, wxPG_LABEL, str);
		}
		else if (type == PropertyType::VEC4)
		{
			std::string str = AssetUtils::ToString(material->GetParameter(nameHash).GetVec4());
			property = new wxStringProperty(label, wxPG_LABEL, str);
		}
				
		auto data = new MaterialEditorPropertyData;
		data->paramaterName = parameterName;
		data->type = type;
		property->SetClientData(data);
		propertyGrid->Append(property);
	}
	
	propertyGrid->Append(new wxFileProperty("Diffuse texture", wxPG_LABEL, material->GetTexture(TextureUnit::DIFFUSE) ? material->GetTexture(TextureUnit::DIFFUSE)->GetFileName() : ""));
	propertyGrid->Append(new wxFileProperty("Normal texture", wxPG_LABEL, material->GetTexture(TextureUnit::NORMAL) ? material->GetTexture(TextureUnit::NORMAL)->GetFileName() : ""));
	propertyGrid->Append(new wxFileProperty("Specular texture", wxPG_LABEL, material->GetTexture(TextureUnit::SPECULAR) ? material->GetTexture(TextureUnit::SPECULAR)->GetFileName() : ""));
	propertyGrid->Append(new wxFileProperty("Technique", wxPG_LABEL, material->GetTechnique() ? material->GetTechnique()->GetFileName() : ""));

	propertyGrid->Refresh();
}

void MaterialEditorPanelImpl::PropertyGridChanged(wxPropertyGridEvent& event)
{	
	std::string properyName = event.GetPropertyName();

	if (properyName == "Name")
	{
		material->SetName(event.GetPropertyValue().GetString().c_str().AsChar());
	}
	else if (properyName == "Diffuse texture")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = AssetUtils::ImportTextureIfNeeded(sfile);
		event.GetProperty()->SetValue(sfile);
		material->SetTexture(TextureUnit::DIFFUSE, engine->GetResourceCache()->GetResource<Texture2D>(sfile));
	}
	else if (properyName == "Normal texture")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = AssetUtils::ImportTextureIfNeeded(sfile);
		event.GetProperty()->SetValue(sfile);
		material->SetTexture(TextureUnit::NORMAL, engine->GetResourceCache()->GetResource<Texture2D>(sfile));
	}
	else if (properyName == "Specular texture")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = AssetUtils::ImportTextureIfNeeded(sfile);
		event.GetProperty()->SetValue(sfile);
		material->SetTexture(TextureUnit::SPECULAR, engine->GetResourceCache()->GetResource<Texture2D>(sfile));
	}
	else if (properyName == "Technique")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = AssetUtils::ImportTechniqueIfNeeded(sfile);
		event.GetProperty()->SetValue(sfile);
		material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>(sfile));
	}
	else
	{
		auto property = event.GetProperty();		
		MaterialEditorPropertyData *data = (MaterialEditorPropertyData *) property->GetClientData();
		if (data->type == PropertyType::COLOR)
		{
			wxColor col = ((wxColourProperty*)event.GetProperty())->GetVal().m_colour;
			material->SetParameter(data->paramaterName, vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
		}
		else if (data->type == PropertyType::FLOAT)
		{
			material->SetParameter(data->paramaterName, (float)event.GetPropertyValue().GetDouble());
		}
		else if (data->type == PropertyType::VEC2)
		{
			material->SetParameter(data->paramaterName, Variant::FromString(event.GetPropertyValue().GetString().ToStdString()).GetVec2());
		}
		else if (data->type == PropertyType::VEC3)
		{
			material->SetParameter(data->paramaterName, Variant::FromString(event.GetPropertyValue().GetString().ToStdString()).GetVec3());
		}
		else if (data->type == PropertyType::VEC4)
		{
			material->SetParameter(data->paramaterName, Variant::FromString(event.GetPropertyValue().GetString().ToStdString()).GetVec4());
		}
	}
	
	if (!currentFileName.empty())
	{
		AssetUtils::SerializeMaterial(material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName);
	}

	editorState->sceneChanged(editorState);
}

void MaterialEditorPanelImpl::MaterialPicked(FireCube::Material *material)
{
	this->material = material;
	currentFileName = material->GetFileName();
	FillPropertyGrid(material);
}

void MaterialEditorPanelImpl::PropertyGridRightClicked(wxPropertyGridEvent& event)
{
	auto property = event.GetProperty();
	MaterialEditorPropertyData *data = (MaterialEditorPropertyData *)property->GetClientData();
	std::string propertyName = event.GetPropertyName();
	wxMenu* menu = new wxMenu;
	auto removePropertyItem = menu->Append(wxID_ANY, wxT("Remove"));
	wxMenuItem *setFromColorItem = nullptr;

	if (propertyName == "Name" || propertyName == "Diffuse texture" || propertyName == "Normal texture" || propertyName == "Specular texture" || propertyName == "Technique")
	{
		removePropertyItem->Enable(false);
	}

	if (data->type == PropertyType::VEC3)
	{
		setFromColorItem = menu->Append(wxID_ANY, wxT("Set From Color"));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, [removePropertyItem, setFromColorItem, this, property, data](wxCommandEvent &event) {
		if (event.GetId() == removePropertyItem->GetId())
		{			
			material->RemoveParameter(data->paramaterName);
			propertyGrid->DeleteProperty(property);
		}
		else if (setFromColorItem && event.GetId() == setFromColorItem->GetId())
		{
			vec3 color = material->GetParameter(data->paramaterName).GetVec3();
			wxColor startColor((unsigned char)(color.x * 255.0f), (unsigned char)(color.y * 255.0f), (unsigned char)(color.z * 255.0f));
			wxColor selectedColor = wxGetColourFromUser(this, startColor, "Select Color");
			if (selectedColor.IsOk())
			{
				color = vec3((float)selectedColor.Red() / 255.0f, (float)selectedColor.Green() / 255.0f, (float)selectedColor.Blue() / 255.0f);
				material->SetParameter(data->paramaterName, color);
				property->SetValue(AssetUtils::ToString(color));
			}			
		}
	});
	PopupMenu(menu);
	delete menu;
}

void MaterialEditorPanelImpl::AddParameterButtonClicked(wxCommandEvent& event)
{
	wxMenu* menu = new wxMenu;
	auto addFloatPropertyItem = menu->Append(wxID_ANY, wxT("Float"));
	auto addVec2PropertyItem = menu->Append(wxID_ANY, wxT("Vec2"));
	auto addVec3PropertyItem = menu->Append(wxID_ANY, wxT("Vec3"));
	auto addVec4PropertyItem = menu->Append(wxID_ANY, wxT("Vec4"));	
	
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, [addFloatPropertyItem, addVec2PropertyItem, addVec3PropertyItem, addVec4PropertyItem, this](wxCommandEvent &event) {
		std::string parameterName = wxGetTextFromUser("Enter name of parameter", "New Parameter").ToStdString();
		if (parameterName.empty() == false)
		{
			wxPGProperty *property;
			PropertyType type;
			if (event.GetId() == addFloatPropertyItem->GetId())
			{
				material->SetParameter(parameterName, 0.0f);
				property = new wxFloatProperty(parameterName, wxPG_LABEL, 0);
				type = PropertyType::FLOAT;
			}
			else if (event.GetId() == addVec2PropertyItem->GetId())
			{
				material->SetParameter(parameterName, vec2(0.0f));
				property = new wxStringProperty(parameterName, wxPG_LABEL, "0 0");
				type = PropertyType::VEC2;
			}
			else if (event.GetId() == addVec3PropertyItem->GetId())
			{
				material->SetParameter(parameterName, vec3(0.0f));
				property = new wxStringProperty(parameterName, wxPG_LABEL, "0 0 0");
				type = PropertyType::VEC2;
			}
			else if (event.GetId() == addVec4PropertyItem->GetId())
			{
				material->SetParameter(parameterName, vec4(0.0f));
				property = new wxStringProperty(parameterName, wxPG_LABEL, "0 0 0");
				type = PropertyType::VEC2;
			}
			
			auto data = new MaterialEditorPropertyData;
			data->paramaterName = parameterName;
			data->type = type;
			property->SetClientData(data);
			propertyGrid->Append(property);
		}
	});
	PopupMenu(menu);
	delete menu;
}

MaterialEditorDropTarget::MaterialEditorDropTarget(MaterialEditorPanelImpl *materialEditorPanel) : wxDropTarget(new wxCustomDataObject(wxDataFormat("Asset"))), materialEditorPanel(materialEditorPanel)
{

}

wxDragResult MaterialEditorDropTarget::OnData(wxCoord vX, wxCoord vY, wxDragResult eResult)
{
	if (GetData())
	{
		auto dataObject = (wxCustomDataObject*)GetDataObject();
		AssetType type;
		std::string path;
		AssetUtils::DeserializeAssetDescription((char *)dataObject->GetData(), type, path);
		std::replace(path.begin(), path.end(), '\\', '/');
		path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

		if (type == AssetType::TEXTURE)
		{
			bool validTarget = false;
			auto pos = materialEditorPanel->propertyGrid->CalcScrolledPosition(wxPoint(vX, vY));
			auto hitTestResult = materialEditorPanel->propertyGrid->HitTest(pos);
			TextureUnit textureUnit;

			if (hitTestResult.GetProperty()->GetName() == "Diffuse texture")
			{
				textureUnit = TextureUnit::DIFFUSE;
				validTarget = true;
			}
			else if (hitTestResult.GetProperty()->GetName() == "Normal texture")
			{
				textureUnit = TextureUnit::NORMAL;
				validTarget = true;
			}
			else if (hitTestResult.GetProperty()->GetName() == "Specular texture")
			{
				textureUnit = TextureUnit::SPECULAR;
				validTarget = true;
			}

			if (validTarget)
			{				
				hitTestResult.GetProperty()->SetValue(path);
				materialEditorPanel->material->SetTexture(textureUnit, materialEditorPanel->engine->GetResourceCache()->GetResource<Texture2D>(path));

				materialEditorPanel->editorState->sceneChanged(materialEditorPanel->editorState);
				if (!materialEditorPanel->currentFileName.empty())
				{
					AssetUtils::SerializeMaterial(materialEditorPanel->material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + materialEditorPanel->currentFileName);
				}
			}
		}
		else if (type == AssetType::TECHNIQUE)
		{			
			auto pos = materialEditorPanel->propertyGrid->CalcScrolledPosition(wxPoint(vX, vY));
			auto hitTestResult = materialEditorPanel->propertyGrid->HitTest(pos);
			if (hitTestResult.GetProperty()->GetName() == "Technique")
			{
				hitTestResult.GetProperty()->SetValue(path);
				materialEditorPanel->material->SetTechnique(materialEditorPanel->engine->GetResourceCache()->GetResource<Technique>(path));

				materialEditorPanel->editorState->sceneChanged(materialEditorPanel->editorState);
				if (!materialEditorPanel->currentFileName.empty())
				{
					AssetUtils::SerializeMaterial(materialEditorPanel->material, Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + materialEditorPanel->currentFileName);
				}
			}
		}
	}

	return eResult;
}

wxDragResult MaterialEditorDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
{	
	auto pos = materialEditorPanel->propertyGrid->CalcScrolledPosition(wxPoint(x, y));
	auto hitTestResult = materialEditorPanel->propertyGrid->HitTest(pos);	

	if (hitTestResult.GetProperty())
	{
		std::string propertyName = hitTestResult.GetProperty()->GetName();
		if (propertyName == "Diffuse texture" || propertyName == "Normal texture" || propertyName == "Specular texture" || propertyName == "Technique")
		{
			return wxDragCopy;
		}
	}
	
	return wxDragNone;
}

wxDragResult MaterialEditorDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult def)
{
	return OnDragOver(x, y, def);
}
