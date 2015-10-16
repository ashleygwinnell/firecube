#include <FireCube.h>
using namespace FireCube;

#include "wx/wx.h"
#include "wx/colordlg.h"
#include "wx/filedlg.h"
#include <wx/glcanvas.h>
#include "Document.h"
#include "GLCanvas.h"
#include "MyMainFrame.h"
#include "main.h"


MyMainFrame::MyMainFrame( wxWindow* parent ) : MainFrame( parent ), theApp((MyApp*)wxTheApp)
{
	this->Connect(propertyGrid1->GetId(), wxEVT_PG_CHANGED, wxCommandEventHandler(MyMainFrame::PropertyGrid1Changed));	
}

void MyMainFrame::MenuItem1Clicked( wxCommandEvent& event )
{
	Close();
}

void MyMainFrame::MenuItem2Clicked( wxCommandEvent& event )
{
	wxString file = wxFileSelector(wxT("Open"), "", "", wxT("3ds,dae,obj,fbx"), wxT("*.3ds;*.dae;*.obj;*.fbx"));
	std::string sfile = file;
	if (sfile == "")
		return;
	std::string::size_type d = sfile.find_last_of("\\");
	theApp->LoadDocument(sfile);
	glCanvas->SetRenderingNormals(glCanvas->GetRenderingNormals());
}

void MyMainFrame::CheckBox2Clicked( wxCommandEvent& event )
{
	glCanvas->SetRenderingNormals(!glCanvas->GetRenderingNormals());
	glCanvas->Refresh();
}

void MyMainFrame::CheckBox4Clicked( wxCommandEvent& event )
{
	glCanvas->SetRenderingTangents(!glCanvas->GetRenderingTangents());
	glCanvas->Refresh();
}

void MyMainFrame::RadioBox1Clicked( wxCommandEvent& event )
{
	int selected = radioBox1->GetSelection();
	if (selected == 0)
		glCanvas->SetRenderingMode(GL_POINT);
	else if (selected == 1)
		glCanvas->SetRenderingMode(GL_LINE);
	else if (selected == 2)
		glCanvas->SetRenderingMode(GL_FILL);

	glCanvas->Refresh();
}

void MyMainFrame::TextCtrl3TextEnter( wxCommandEvent& event )
{
	double l;
	if (textCtrl3->GetValue().ToDouble(&l))
	{
		theApp->GetApplicationParameters().normalsLength = (float)l;        
		theApp->GetDocument().GenerateNormals(theApp->GetApplicationParameters().normalsLength);
		theApp->GetDocument().GenerateTangents(theApp->GetApplicationParameters().normalsLength);		
		glCanvas->Refresh();
	}
}

void MyMainFrame::CheckBox3Clicked( wxCommandEvent& event )
{
	glCanvas->SetCullFace(!glCanvas->GetCullFace());
	glCanvas->Refresh();
}

void MyMainFrame::ColourPicker1Changed( wxColourPickerEvent& event )
{
	wxColor color = colourPicker1->GetColour();
	glCanvas->SetBackgroundColor(vec3(color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f));
	glCanvas->Refresh();
}

void MyMainFrame::Button1Clicked( wxCommandEvent& event )
{
	glCanvas->SetRotation(vec3(0, 0, 0.0f));
	glCanvas->SetDistance(5.0f);
	glCanvas->Refresh();
}

void MyMainFrame::Button2Clicked( wxCommandEvent& event )
{
	std::string dir = wxDirSelector().ToStdString();
	if (dir.empty() == false)
	{
		//Filesystem::AddSearchPath(dir);
	}
}

void MyMainFrame::TextCtrl4TextEnter( wxCommandEvent& event )
{
	double size;
	long numberOfCells;
	if (textCtrl4->GetValue().ToDouble(&size))
	{
		if (size > 0.0f)
		{
			if (textCtrl5->GetValue().ToLong(&numberOfCells))
			{
				if (numberOfCells > 0)
				{					
					theApp->GetDocument().CreateGrid((float)size, (DWORD)numberOfCells);
					glCanvas->Refresh();
				}
			}
		}
	}
}

void MyMainFrame::TextCtrl5TextEnter( wxCommandEvent& event )
{
	double size;
	long numberOfCells;
	if (textCtrl4->GetValue().ToDouble(&size))
	{
		if (size > 0.0f)
		{
			if (textCtrl5->GetValue().ToLong(&numberOfCells))
			{
				if (numberOfCells > 0)
				{
					theApp->GetDocument().CreateGrid((float)size, (DWORD)numberOfCells);
					glCanvas->Refresh();
				}
			}
		}
	}
}

void MyMainFrame::MenuItem5Clicked( wxCommandEvent& event )
{
	if (statusBar1->IsShown())
		statusBar1->Hide();
	else
		statusBar1->Show();

	Layout();
}

void MyMainFrame::MenuItem6Clicked( wxCommandEvent& event )
{
	if (notebook1->IsShown())
		notebook1->Hide();
	else
		notebook1->Show();

	Layout();
}

bool MyMainFrame::AddMaterial(DWORD id, Material *mat)
{	
	for (auto i = materialMap.begin(); i != materialMap.end(); i++)
	{
		if (i->second == mat)					
			return false;
	}
	materialMap[id] = mat;
	std::ostringstream oss;
	oss << "Material " << id;
	propertyGrid1->Append(new wxPropertyCategory(oss.str()) );

	std::ostringstream ossName;
	ossName << "Name" << id;
	propertyGrid1->Append(new wxStringProperty("Name", ossName.str(), mat->GetName()));

	if (mat->HasParameter(PARAM_MATERIAL_DIFFUSE))
	{
		vec3 color = mat->GetParameter(PARAM_MATERIAL_DIFFUSE).GetVec3();
		std::ostringstream ossDiffuse;
		ossDiffuse << "Diffuse" << id;
		propertyGrid1->Append(new wxColourProperty("Diffuse", ossDiffuse.str(), wxColor(color.x * 255, color.y * 255, color.z * 255)));
	}
	if (mat->HasParameter(PARAM_MATERIAL_SPECULAR))
	{
		vec3 color = mat->GetParameter(PARAM_MATERIAL_SPECULAR).GetVec3();
		std::ostringstream ossSpecular;
		ossSpecular << "Specular" << id;
		propertyGrid1->Append(new wxColourProperty("Specular", ossSpecular.str(), wxColor(color.x * 255, color.y * 255, color.z * 255)));
	}	
	if (mat->HasParameter(PARAM_MATERIAL_SHININESS))
	{
		float value = mat->GetParameter(PARAM_MATERIAL_SHININESS).GetFloat();
		std::ostringstream ossShininess;
		ossShininess << "Shininess" << id;
		propertyGrid1->Append(new wxFloatProperty("Shininess", ossShininess.str(), value) );
	}				
	if (mat->HasParameter(PARAM_MATERIAL_OPACITY))
	{
		float value = mat->GetParameter(PARAM_MATERIAL_OPACITY).GetFloat();
		std::ostringstream ossOpacity;
		ossOpacity << "Opacity" << id;
		propertyGrid1->Append(new wxFloatProperty("Opacity", ossOpacity.str(), value));
	}
	std::ostringstream ossDiffuseTexture;
	ossDiffuseTexture << "TextureDiffuse" << id;
	propertyGrid1->Append(new wxFileProperty("Diffuse texture", ossDiffuseTexture.str(), mat->GetTexture(TextureUnit::DIFFUSE) ? mat->GetTexture(TextureUnit::DIFFUSE)->GetFileName() : ""));
	std::ostringstream ossNormalTexture;
	ossNormalTexture << "TextureNormal" << id;
	propertyGrid1->Append(new wxFileProperty("Normal texture", ossNormalTexture.str(), mat->GetTexture(TextureUnit::NORMAL) ? mat->GetTexture(TextureUnit::NORMAL)->GetFileName() : ""));
	std::ostringstream ossTechnique;
	ossTechnique << "Technique" << id;
	propertyGrid1->Append(new wxFileProperty("Technique", ossTechnique.str(), mat->GetTechnique() ? mat->GetTechnique()->GetFileName() : ""));

	propertyGrid1->Refresh();
	return true;
}

void MyMainFrame::PropertyGrid1Changed(wxCommandEvent& event )
{
	bool  updateTechnique = false;
	wxPropertyGridEvent *evt = (wxPropertyGridEvent *)&event;
	std::string properyName = evt->GetPropertyName();
	Material *mat;
	if (properyName.substr(0, 4) == "Name")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(4));
		idss >> id;
		mat = materialMap[id];
		
		mat->SetName(evt->GetPropertyValue().GetString().c_str().AsChar());
	}
	if (properyName.substr(0, 7) == "Diffuse")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(7));
		idss >> id;
		mat = materialMap[id];
		wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
		auto i = mat->GetParameters().find(PARAM_MATERIAL_DIFFUSE);
		if (i != mat->GetParameters().end())
			i->second = vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f);
	}
	if (properyName.substr(0, 8) == "Specular")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(8));
		idss >> id;
		mat = materialMap[id];
		wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
		auto i = mat->GetParameters().find(PARAM_MATERIAL_SPECULAR);
		if (i != mat->GetParameters().end())
			i->second = vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f);
	}
	if (properyName.substr(0, 9) == "Shininess")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(9));
		idss >> id;
		mat = materialMap[id];
		auto i = mat->GetParameters().find(PARAM_MATERIAL_SHININESS);
		if (i != mat->GetParameters().end())
			i->second = (float) evt->GetPropertyValue().GetDouble();		
	}
	if (properyName.substr(0, 7) == "Opacity")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(7));
		idss >> id;
		mat = materialMap[id];
		auto i = mat->GetParameters().find(PARAM_MATERIAL_OPACITY);
		if (i != mat->GetParameters().end())
			i->second = (float)evt->GetPropertyValue().GetDouble();
	}
	if (properyName.substr(0, 14) == "TextureDiffuse")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(14));
		idss >> id;
		mat = materialMap[id];
		mat->SetTexture(TextureUnit::DIFFUSE, theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Texture>(evt->GetPropertyValue().GetString().ToStdString()));
		updateTechnique = true;
	}
	if (properyName.substr(0, 13) == "TextureNormal")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(13));
		idss >> id;
		mat = materialMap[id];		
		mat->SetTexture(TextureUnit::NORMAL, theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Texture>(evt->GetPropertyValue().GetString().ToStdString()));
		updateTechnique = true;
	}
	if (properyName.substr(0, 9) == "Technique")
	{
		DWORD id;
		std::istringstream idss(properyName.substr(9));
		idss >> id;
		mat = materialMap[id];		
		mat->SetTechnique(theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Technique>(evt->GetPropertyValue().GetString().ToStdString()));
	}
	if (updateTechnique && mat)
	{
		if (mat->GetTexture(TextureUnit::DIFFUSE) && mat->GetTexture(TextureUnit::NORMAL))
			mat->SetTechnique(theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseNormalMap.xml"));
		else if (mat->GetTexture(TextureUnit::DIFFUSE))
			mat->SetTechnique(theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseMap.xml"));
		else if (mat->GetTexture(TextureUnit::NORMAL))
			mat->SetTechnique(theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Technique>("Techniques/NormalMap.xml"));
		else
			mat->SetTechnique(theApp->fcApp.GetEngine()->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
	}
	
	glCanvas->Refresh();	
}

void MyMainFrame::PopulateTreeWithNode(FireCube::Node *node, wxTreeCtrl *tree)
{
	tree->DeleteAllItems();
	TreeItemData *data = new TreeItemData;
	data->node = node;
	wxTreeItemId id = tree->AddRoot("root", -1, -1, data);
	PopulateTreeWithNode(node, id, tree);
}

void MyMainFrame::PopulateTreeWithNode(FireCube::Node *node, wxTreeItemId treeNode, wxTreeCtrl *tree)
{
	for (unsigned int i = 0; i < node->GetChildren().size(); i++)
	{
		Node *child = node->GetChildren()[i];
		TreeItemData *data = new TreeItemData;
		data->node = child;
		wxTreeItemId id = tree->AppendItem(treeNode, child->GetName(), -1, -1, data);
		PopulateTreeWithNode(child, id, tree);
	}
}

void MyMainFrame::UpdateUI(Document &document)
{
	if (document.HasTangents())
	{
		glCanvas->SetRenderingTangents(checkBox4->GetValue());
		checkBox4->Enable(true);
	}
	else
	{
		glCanvas->SetRenderingTangents(false);
		checkBox4->Enable(false);
	}
	std::ostringstream oss, oss2;
	oss << document.GetVertexCount();
	oss2 << document.GetFaceCount();
	textCtrl1->SetValue(oss.str());
	textCtrl2->SetValue(oss2.str());
	propertyGrid1->Clear();	
	AddMaterials(document.GetAllMaterials());
	PopulateTreeWithNode(document.GetRoot(), treeCtrl2);
}

void MyMainFrame::SetStatusBarText(const std::string &text)
{
	statusBar1->SetStatusText(text);
}

GLCanvas *MyMainFrame::GetMainGLCanvas()
{
	return glCanvas;
}

void MyMainFrame::AddMaterials(const std::vector<FireCube::SharedPtr<FireCube::Material>> &materials)
{	
	unsigned int id = 1;
	for (auto m : materials)
	{
		AddMaterial(id++, m);
	}
}
