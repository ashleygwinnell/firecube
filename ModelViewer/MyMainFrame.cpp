#include <FireCube.h>
using namespace std;

#include "wx/wx.h"
#include "wx/colordlg.h"
#include <wx/glcanvas.h>
#include "MyGLCanvas.h"
#include "MyMainFrame.h"
#include "main.h"


MyMainFrame::MyMainFrame( wxWindow* parent )
    :
    MainFrame( parent )
{
    this->Connect(propertyGrid1->GetId(), wxEVT_PG_CHANGED, wxCommandEventHandler(MyMainFrame::PropertyGrid1Changed));


}
void MyMainFrame::MenuItem1Clicked( wxCommandEvent& event )
{
    ((MyApp*)wxTheApp)->frame->Close();
}
void MyMainFrame::MenuItem2Clicked( wxCommandEvent& event )
{
    wxString file = wxFileSelector(wxT("Open"), 0, 0, wxT("3ds,dae"), wxT("*.3ds;*.dae"));
    FireCubeApp *fcApp = &(((MyApp*)wxTheApp)->fireCubeApp);
    std::string sfile = file;
    if (sfile == "")
        return;
    string::size_type d = sfile.find_last_of("\\");
    fcApp->LoadModel(sfile);
}
void MyMainFrame::MenuItem3Clicked( wxCommandEvent& event )
{
    FireCubeApp *fcApp = &(((MyApp*)wxTheApp)->fireCubeApp);
    wxString file = wxFileSelector(wxT("Open"), 0, 0, wxT("vert"), wxT("*.vert"));
    std::string sfile = file;
    if (sfile == "")
        return;
    fcApp->vshader = FireCube::Renderer::GetShaderManager().Create(sfile);
    fcApp->program->Create(fcApp->vshader, fcApp->fshader);
    glCanvas->customProgram = true;
    fcApp->root->SetProgram(fcApp->program);
}
void MyMainFrame::MenuItem4Clicked( wxCommandEvent& event )
{
    FireCubeApp *fcApp = &(((MyApp*)wxTheApp)->fireCubeApp);
    wxString file = wxFileSelector(wxT("Open"), 0, 0, wxT("frag"), wxT("*.frag"));
    FireCubeApp *app = &(((MyApp*)wxTheApp)->fireCubeApp);
    std::string sfile = file;
    if (sfile == "")
        return;
    fcApp->fshader = FireCube::Renderer::GetShaderManager().Create(sfile);
    fcApp->program->Create(fcApp->vshader, fcApp->fshader);
    glCanvas->customProgram = true;
    fcApp->root->SetProgram(fcApp->program);
}
void MyMainFrame::MenuItem7Clicked( wxCommandEvent& event )
{
    FireCubeApp *fcApp = &(((MyApp*)wxTheApp)->fireCubeApp);
    FireCubeApp *app = &(((MyApp*)wxTheApp)->fireCubeApp);
    glCanvas->customProgram = false;
    fcApp->root->SetProgram(FireCube::Program());
}
void MyMainFrame::CheckBox2Clicked( wxCommandEvent& event )
{
    glCanvas->renderNormals = !glCanvas->renderNormals;
    glCanvas->Refresh();
}
void MyMainFrame::CheckBox4Clicked( wxCommandEvent& event )
{
    glCanvas->renderTangents = !glCanvas->renderTangents;
    glCanvas->Refresh();
}
void MyMainFrame::RadioBox1Clicked( wxCommandEvent& event )
{
    int selected = radioBox1->GetSelection();
    if (selected == 0)
        glCanvas->renderingMode = GL_POINT;
    else if (selected == 1)
        glCanvas->renderingMode = GL_LINE;
    else if (selected == 2)
        glCanvas->renderingMode = GL_FILL;

    glCanvas->Refresh();
}
void MyMainFrame::TextCtrl3TextEnter( wxCommandEvent& event )
{
    double l;
    if (textCtrl3->GetValue().ToDouble(&l))
    {
        glCanvas->normalsLength = (float)l;
        MyApp *app = (MyApp*)wxTheApp;
        app->fireCubeApp.GenerateNormals(glCanvas->normalsLength);
        app->fireCubeApp.GenerateTangents(glCanvas->normalsLength);
        app->fireCubeApp.GenerateBitangents(glCanvas->normalsLength);
        glCanvas->Refresh();
    }
}
void MyMainFrame::CheckBox3Clicked( wxCommandEvent& event )
{
    glCanvas->cullFaceEnabled = !glCanvas->cullFaceEnabled;
    glCanvas->Refresh();
}
void MyMainFrame::ColourPicker1Changed( wxColourPickerEvent& event )
{
    wxColor color = colourPicker1->GetColour();
    glCanvas->bgColor = FireCube::vec4(color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f, 1.0f);
    glCanvas->Refresh();
}
void MyMainFrame::Button1Clicked( wxCommandEvent& event )
{
    glCanvas->rot = FireCube::vec3(0, 0, -5);
    glCanvas->lookat = FireCube::vec3(0, 0, 0);
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
void MyMainFrame::AddMaterial(DWORD id, FireCube::Material mat)
{
    materialMap[id] = mat;
    ostringstream oss;
    oss << "Material " << id;
    propertyGrid1->Append(new wxPropertyCategory(oss.str()) );

    ostringstream ossName;
    ossName << "Name" << id;
    propertyGrid1->Append(new wxStringProperty("Name", ossName.str(), mat->GetName()));
    ostringstream ossAmbient;
    ossAmbient << "Ambient" << id;
    propertyGrid1->Append(new wxColourProperty("Ambient", ossAmbient.str(), wxColor(mat->GetAmbientColor().x * 255, mat->GetAmbientColor().y * 255, mat->GetAmbientColor().z * 255, mat->GetAmbientColor().w * 255)));
    ostringstream ossDiffuse;
    ossDiffuse << "Diffuse" << id;
    propertyGrid1->Append(new wxColourProperty("Diffuse", ossDiffuse.str(), wxColor(mat->GetDiffuseColor().x * 255, mat->GetDiffuseColor().y * 255, mat->GetDiffuseColor().z * 255, mat->GetDiffuseColor().w * 255)));
    ostringstream ossSpecular;
    ossSpecular << "Specular" << id;
    propertyGrid1->Append(new wxColourProperty("Specular", ossSpecular.str(), wxColor(mat->GetSpecularColor().x * 255, mat->GetSpecularColor().y * 255, mat->GetSpecularColor().z * 255, mat->GetSpecularColor().w * 255)));
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
        FireCube::Material mat = materialMap[id];
        mat->SetName(evt->GetPropertyValueAsString().c_str());
    }
    if (properyName.substr(0, 7) == "Ambient")
    {
        DWORD id;
        istringstream idss(properyName.substr(7));
        idss >> id;
        FireCube::Material mat = materialMap[id];
        wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
        mat->SetAmbientColor(FireCube::vec4(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, mat->GetAmbientColor().w));
    }
    if (properyName.substr(0, 7) == "Diffuse")
    {
        DWORD id;
        istringstream idss(properyName.substr(7));
        idss >> id;
        FireCube::Material mat = materialMap[id];
        wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
        mat->SetDiffuseColor(FireCube::vec4(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, mat->GetDiffuseColor().w));
    }
    if (properyName.substr(0, 8) == "Specular")
    {
        DWORD id;
        istringstream idss(properyName.substr(8));
        idss >> id;
        FireCube::Material mat = materialMap[id];
        wxColor col = ((wxColourProperty*)evt->GetProperty())->GetVal().m_colour;
        mat->SetSpecularColor(FireCube::vec4(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, mat->GetSpecularColor().w));
    }
    if (properyName.substr(0, 9) == "Shininess")
    {
        DWORD id;
        istringstream idss(properyName.substr(9));
        idss >> id;
        FireCube::Material mat = materialMap[id];
        mat->SetShininess((float)evt->GetPropertyValueAsDouble());
    }
    if (properyName.substr(0, 14) == "TextureDiffuse")
    {
        DWORD id;
        istringstream idss(properyName.substr(14));
        idss >> id;
        FireCube::Material mat = materialMap[id];
        mat->SetDiffuseTexture(FireCube::Renderer::GetTextureManager().Create(evt->GetPropertyValueAsString().ToAscii()));
    }
    if (properyName.substr(0, 13) == "TextureNormal")
    {
        DWORD id;
        istringstream idss(properyName.substr(13));
        idss >> id;
        FireCube::Material mat = materialMap[id];
        mat->SetNormalTexture(FireCube::Renderer::GetTextureManager().Create(evt->GetPropertyValueAsString().ToAscii()));
    }
    glCanvas->Refresh();
}