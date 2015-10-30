#include "MaterialEditorPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../AssetUtils.h"

using namespace FireCube;

MaterialEditorPanelImpl::MaterialEditorPanelImpl(wxWindow* parent) : MaterialEditorPanel(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), editorState(((MyApp*)wxTheApp)->GetEditorState())
{
	
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

void MaterialEditorPanelImpl::FillPropertyGrid(Material *material)
{	
	vec3 color;
	float value;
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
	
	propertyGrid->Append(new wxFileProperty("Diffuse texture", wxPG_LABEL, material->GetTexture(TextureUnit::DIFFUSE) ? material->GetTexture(TextureUnit::DIFFUSE)->GetFileName() : ""));
	propertyGrid->Append(new wxFileProperty("Normal texture", wxPG_LABEL, material->GetTexture(TextureUnit::NORMAL) ? material->GetTexture(TextureUnit::NORMAL)->GetFileName() : ""));
	propertyGrid->Append(new wxFileProperty("Specular texture", wxPG_LABEL, material->GetTexture(TextureUnit::SPECULAR) ? material->GetTexture(TextureUnit::SPECULAR)->GetFileName() : ""));
	propertyGrid->Append(new wxFileProperty("Technique", wxPG_LABEL, material->GetTechnique() ? material->GetTechnique()->GetFileName() : ""));

	propertyGrid->Refresh();
}

std::string ImportTextureIfNeeded(const std::string &textuerPath)
{
	std::string sfile = textuerPath;

	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
	{
		sfile = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);
	}
	else
	{
		Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures");

		std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
		Filesystem::CopyPath(sfile, targetPath);
		sfile = "Textures" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
	}

	return sfile;
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
	else if (properyName == "Diffuse texture")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = ImportTextureIfNeeded(sfile);
		std::replace(sfile.begin(), sfile.end(), '\\', '/');
		event.GetProperty()->SetValue(sfile);
		material->SetTexture(TextureUnit::DIFFUSE, engine->GetResourceCache()->GetResource<Texture>(sfile));
	}
	else if (properyName == "Normal texture")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = ImportTextureIfNeeded(sfile);
		std::replace(sfile.begin(), sfile.end(), '\\', '/');
		event.GetProperty()->SetValue(sfile);
		material->SetTexture(TextureUnit::NORMAL, engine->GetResourceCache()->GetResource<Texture>(sfile));
	}
	else if (properyName == "Specular texture")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();
		sfile = ImportTextureIfNeeded(sfile);
		std::replace(sfile.begin(), sfile.end(), '\\', '/');
		event.GetProperty()->SetValue(sfile);
		material->SetTexture(TextureUnit::SPECULAR, engine->GetResourceCache()->GetResource<Texture>(sfile));
	}
	else if (properyName == "Technique")
	{
		std::string sfile = event.GetPropertyValue().GetString().ToStdString();		

		if (Filesystem::IsSubPathOf(Filesystem::GetCoreDataFolder(), sfile))
		{
			sfile = Filesystem::MakeRelativeTo(Filesystem::GetCoreDataFolder(), sfile);
		}
		else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
		{
			sfile = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);
		}
		else
		{
			Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Techniques");

			std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Techniques" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
			Filesystem::CopyPath(sfile, targetPath);
			sfile = "Techniques" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
		}
		std::replace(sfile.begin(), sfile.end(), '\\', '/');
		event.GetProperty()->SetValue(sfile);
		material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>(sfile));
	}	

	editorState->sceneChanged(editorState);
}