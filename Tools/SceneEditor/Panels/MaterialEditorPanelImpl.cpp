#include "MaterialEditorPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../AssetUtils.h"
#include <wx/wrapsizer.h>

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

	Layout();
	this->SetMinSize(wxSize(-1, -1)); // Fix bug where initially the panel has a minimum size set due to the box sizer
}

MaterialEditorPanelImpl::~MaterialEditorPanelImpl()
{

}

void MaterialEditorPanelImpl::NewButtonClicked(wxCommandEvent& event)
{
	material = new Material(engine);
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
	material->SetParameter(PARAM_MATERIAL_SPECULAR, vec3(0.0f));
	material->SetParameter(PARAM_MATERIAL_SHININESS, 0.0f);
	material->SetParameter(PARAM_MATERIAL_OPACITY, 0.0f);
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
	vec3 color;
	float value;
	std::string str;
	propertyGrid->Clear();
	
	propertyGrid->Append(new wxPropertyCategory("Material"));

	propertyGrid->Append(new wxStringProperty("Name", wxPG_LABEL, material->GetName()));

	color = material->HasParameter(PARAM_MATERIAL_DIFFUSE) ? material->GetParameter(PARAM_MATERIAL_DIFFUSE).GetVec3() : vec3(1.0f);
	propertyGrid->Append(new wxColourProperty("Diffuse", wxPG_LABEL, wxColor(color.x * 255, color.y * 255, color.z * 255)));
	
	color = material->HasParameter(PARAM_MATERIAL_SPECULAR) ? material->GetParameter(PARAM_MATERIAL_SPECULAR).GetVec3() : vec3(0.0f);
	propertyGrid->Append(new wxColourProperty("Specular", wxPG_LABEL, wxColor(color.x * 255, color.y * 255, color.z * 255)));
	
	value = material->HasParameter(PARAM_MATERIAL_SHININESS) ? material->GetParameter(PARAM_MATERIAL_SHININESS).GetFloat() : 0.0f;
	propertyGrid->Append(new wxFloatProperty("Shininess", wxPG_LABEL, value));
	
	value = material->HasParameter(PARAM_MATERIAL_OPACITY) ? material->GetParameter(PARAM_MATERIAL_OPACITY).GetFloat() : 1.0f;
	propertyGrid->Append(new wxFloatProperty("Opacity", wxPG_LABEL, value));

	str = material->HasParameter(PARAM_U_OFFSET) ? AssetUtils::ToString(material->GetParameter(PARAM_U_OFFSET).GetVec3()) : "1 0 0";
	propertyGrid->Append(new wxStringProperty("U Offset", wxPG_LABEL, str));

	str = material->HasParameter(PARAM_V_OFFSET) ? AssetUtils::ToString(material->GetParameter(PARAM_V_OFFSET).GetVec3()) : "0 1 0";
	propertyGrid->Append(new wxStringProperty("V Offset", wxPG_LABEL, str));
	
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
	else if (properyName == "Diffuse")
	{		
		wxColor col = ((wxColourProperty*)event.GetProperty())->GetVal().m_colour;
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));		
	}
	else if (properyName == "Specular")
	{
		wxColor col = ((wxColourProperty*)event.GetProperty())->GetVal().m_colour;
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
	}
	else if (properyName == "Shininess")
	{
		material->SetParameter(PARAM_MATERIAL_SHININESS, (float)event.GetPropertyValue().GetDouble());		
	}
	else if (properyName == "Opacity")
	{
		material->SetParameter(PARAM_MATERIAL_OPACITY, (float)event.GetPropertyValue().GetDouble());		
	}
	else if (properyName == "U Offset")
	{
		material->SetParameter(PARAM_U_OFFSET, Variant::FromString(event.GetPropertyValue().GetString().ToStdString()).GetVec3());
	}
	else if (properyName == "V Offset")
	{
		material->SetParameter(PARAM_V_OFFSET, Variant::FromString(event.GetPropertyValue().GetString().ToStdString()).GetVec3());
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
