#include <FireCube.h>
using namespace std;

#include "wx/wx.h"
#include "wx/colordlg.h"
#include <wx/glcanvas.h>
#include "Document.h"
#include "GLCanvas.h"
#include "MyMainFrame.h"
#include "main.h"


MyMainFrame::MyMainFrame( wxWindow* parent )
    :
    MainFrame( parent ), theApp((MyApp*)wxTheApp)
{
    this->Connect(propertyGrid1->GetId(), wxEVT_PG_CHANGED, wxCommandEventHandler(MyMainFrame::PropertyGrid1Changed));


}
void MyMainFrame::MenuItem1Clicked( wxCommandEvent& event )
{
    Close();
}
void MyMainFrame::MenuItem2Clicked( wxCommandEvent& event )
{
    wxString file = wxFileSelector(wxT("Open"), 0, 0, wxT("3ds,dae,obj"), wxT("*.3ds;*.dae;*.obj"));    
    std::string sfile = file;
    if (sfile == "")
        return;
    string::size_type d = sfile.find_last_of("\\");
    theApp->LoadDocument(sfile);
}
void MyMainFrame::MenuItem3Clicked( wxCommandEvent& event )
{    
    wxString file = wxFileSelector(wxT("Open"), 0, 0, wxT("vert"), wxT("*.vert"));
    std::string sfile = file;
    if (sfile == "")
        return;
    theApp->GetApplicationParameters().vshader = FireCube::Renderer::GetShaderManager().Create(sfile);
    theApp->GetApplicationParameters().program->Create(theApp->GetApplicationParameters().vshader, theApp->GetApplicationParameters().fshader);
    theApp->GetApplicationParameters().customProgram = true;
	theApp->GetDocument().GetRoot()->SetProgram(theApp->GetApplicationParameters().program);    
}
void MyMainFrame::MenuItem4Clicked( wxCommandEvent& event )
{    
    wxString file = wxFileSelector(wxT("Open"), 0, 0, wxT("frag"), wxT("*.frag"));
    std::string sfile = file;
    if (sfile == "")
        return;
    theApp->GetApplicationParameters().fshader = FireCube::Renderer::GetShaderManager().Create(sfile);
    theApp->GetApplicationParameters().program->Create(theApp->GetApplicationParameters().vshader, theApp->GetApplicationParameters().fshader);
	theApp->GetApplicationParameters().customProgram = true;
	theApp->GetDocument().GetRoot()->SetProgram(theApp->GetApplicationParameters().program);    
}
void MyMainFrame::MenuItem7Clicked( wxCommandEvent& event )
{
    theApp->GetApplicationParameters().customProgram = false;
    theApp->GetDocument().GetRoot()->SetProgram(FireCube::ProgramPtr());
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
		theApp->GetDocument().GenerateBitangents(theApp->GetApplicationParameters().normalsLength);
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
    glCanvas->SetBackgroundColor(FireCube::vec4(color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f, 1.0f));
    glCanvas->Refresh();
}
void MyMainFrame::Button1Clicked( wxCommandEvent& event )
{
    glCanvas->SetRotation(FireCube::vec3(0, 0, 0.0f));
    glCanvas->SetDistance(5.0f);
    glCanvas->Refresh();
}
void MyMainFrame::Button2Clicked( wxCommandEvent& event )
{
    string dir = wxDirSelector();
    if (dir != "")
    {
        FireCube::Filesystem::AddSearchPath(dir);
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
                    glCanvas->CreateGrid((float)size, (DWORD)numberOfCells);
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
                    glCanvas->CreateGrid((float)size, (DWORD)numberOfCells);
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
bool MyMainFrame::AddMaterial(DWORD id, FireCube::MaterialPtr mat)
{
    for (map<DWORD, FireCube::MaterialPtr>::iterator i = materialMap.begin(); i != materialMap.end(); i++)
	{
		if (i->second == mat)					
			return false;
	}
	materialMap[id] = mat;
    ostringstream oss;
    oss << "Material " << id;
    propertyGrid1->Append(new wxPropertyCategory(oss.str()) );

    ostringstream ossName;
    ossName << "Name" << id;
    propertyGrid1->Append(new wxStringProperty("Name", ossName.str(), mat->GetName()));
    ostringstream ossAmbient;
    ossAmbient << "Ambient" << id;
    propertyGrid1->Append(new wxColourProperty("Ambient", ossAmbient.str(), wxColor(mat->GetAmbientColor().x * 255, mat->GetAmbientColor().y * 255, mat->GetAmbientColor().z * 255)));
    ostringstream ossDiffuse;
    ossDiffuse << "Diffuse" << id;
    propertyGrid1->Append(new wxColourProperty("Diffuse", ossDiffuse.str(), wxColor(mat->GetDiffuseColor().x * 255, mat->GetDiffuseColor().y * 255, mat->GetDiffuseColor().z * 255)));
    ostringstream ossSpecular;
    ossSpecular << "Specular" << id;
    propertyGrid1->Append(new wxColourProperty("Specular", ossSpecular.str(), wxColor(mat->GetSpecularColor().x * 255, mat->GetSpecularColor().y * 255, mat->GetSpecularColor().z * 255)));
    ostringstream ossShininess;
    ossShininess << "Shininess" << id;
    propertyGrid1->Append(new wxFloatProperty("Shininess", ossShininess.str(), mat->GetShininess()) );
    ostringstream ossDiffuseTexture;
    ossDiffuseTexture << "TextureDiffuse" << id;
    propertyGrid1->Append(new wxFileProperty("Diffuse texture", ossDiffuseTexture.str(), mat->GetDiffuseTexture() ? mat->GetDiffuseTexture()->GetFileName() : ""));
    ostringstream ossNormalTexture;
    ossNormalTexture << "TextureNormal" << id;
    propertyGrid1->Append(new wxFileProperty("Normal texture", ossNormalTexture.str(), mat->GetNormalTexture() ? mat->GetNormalTexture()->GetFileName() : ""));

    propertyGrid1->Refresh();
	return true;
}
void MyMainFrame::PropertyGrid1Changed(wxCommandEvent& event )
{
    wxPropertyGridEvent *evt = (wxPropertyGridEvent *)&event;
    string properyName = evt->GetPropertyName();
    if (properyName.substr(0, 4) == "Name")
    {
        DWORD id;
        istringstream idss(properyName.substr(4));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        mat->SetName(evt->GetPropertyValueAsString().c_str());
    }
    if (properyName.substr(0, 7) == "Ambient")
    {
        DWORD id;
        istringstream idss(properyName.substr(7));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
        mat->SetAmbientColor(FireCube::vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
    }
    if (properyName.substr(0, 7) == "Diffuse")
    {
        DWORD id;
        istringstream idss(properyName.substr(7));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
        mat->SetDiffuseColor(FireCube::vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
    }
    if (properyName.substr(0, 8) == "Specular")
    {
        DWORD id;
        istringstream idss(properyName.substr(8));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
        mat->SetSpecularColor(FireCube::vec3(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
    }
    if (properyName.substr(0, 9) == "Shininess")
    {
        DWORD id;
        istringstream idss(properyName.substr(9));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        mat->SetShininess((float)evt->GetPropertyValueAsDouble());
    }
    if (properyName.substr(0, 14) == "TextureDiffuse")
    {
        DWORD id;
        istringstream idss(properyName.substr(14));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        mat->SetDiffuseTexture(FireCube::Renderer::GetTextureManager().Create(evt->GetPropertyValueAsString().ToAscii()));
    }
    if (properyName.substr(0, 13) == "TextureNormal")
    {
        DWORD id;
        istringstream idss(properyName.substr(13));
        idss >> id;
        FireCube::MaterialPtr mat = materialMap[id];
        mat->SetNormalTexture(FireCube::Renderer::GetTextureManager().Create(evt->GetPropertyValueAsString().ToAscii()));
    }
    glCanvas->Refresh();	
}
void MyMainFrame::PopulateTreeWithNode(FireCube::NodePtr node, wxTreeCtrl *tree)
{
	tree->DeleteAllItems();
	TreeItemData *data = new TreeItemData;
	data->node = node;
	wxTreeItemId id = tree->AddRoot("root", -1, -1, data);
	PopulateTreeWithNode(node, id, tree);
}
void MyMainFrame::PopulateTreeWithNode(FireCube::NodePtr node, wxTreeItemId treeNode, wxTreeCtrl *tree)
{
	for (unsigned int i = 0; i < node->GetChildren().size(); i++)
	{
		FireCube::NodePtr child = node->GetChildren()[i];
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
	ostringstream oss, oss2;
	oss << document.GetVertexCount();
	oss2 << document.GetFaceCount();
	textCtrl1->SetValue(oss.str());
	textCtrl2->SetValue(oss2.str());
	propertyGrid1->Clear();
	unsigned int id = 1;
	AddMaterials(id, document.GetRoot());
	PopulateTreeWithNode(document.GetRoot(), treeCtrl2);
}

void MyMainFrame::SetStatusBarText(const string &text)
{
	statusBar1->SetStatusText(text);
}
GLCanvas *MyMainFrame::GetMainGLCanvas()
{
	return glCanvas;
}
void MyMainFrame::AddMaterials(unsigned int &id, FireCube::NodePtr node)
{	
	if (node->GetGeometry())
	{        
		if (AddMaterial(id, node->GetGeometry()->GetMaterial()))
			id++;
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		AddMaterials(id, *i);
}
