#include "MainFrameImpl.h"
#include <wx/msgdlg.h>
#include "wx/filedlg.h"
#include "app.h"
#include "Types.h"
#include "SceneDescriptor.h"
#include "Commands/AddNodeCommand.h"
using namespace FireCube;

MainFrameImpl::MainFrameImpl(wxWindow* parent) : MainFrame(parent), theApp((MyApp*)wxTheApp), editorState(theApp->GetEditorState())
{

}

void MainFrameImpl::MyButtonClicked( wxCommandEvent& event )
{
	NodeDescriptor *nodeDescriptor = new NodeDescriptor(engine, sceneDescriptor, "TestNode");	
	StaticModel *model = nodeDescriptor->GetNode()->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh(new Mesh(engine));
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);	
	editorState->ExecuteCommand(new AddNodeCommand(editorState, nodeDescriptor, rootDescriptor));
	this->glCanvas->Refresh(false);
	this->glCanvas->SetFocus();
}

void MainFrameImpl::LoadMeshClicked(wxCommandEvent& event)
{
	wxString file = wxFileSelector(wxT("Open"), "", "", wxT("3ds,dae,obj,fbx"), wxT("*.3ds;*.dae;*.obj;*.fbx"));
	std::string sfile = file;
	if (sfile == "")
		return;

	NodeDescriptor *nodeDescriptor = new NodeDescriptor(engine, sceneDescriptor, "TestNode");
	StaticModel *model = nodeDescriptor->GetNode()->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(sfile));
	model->SetCollisionQueryMask(USER_GEOMETRY);
	editorState->ExecuteCommand(new AddNodeCommand(editorState, nodeDescriptor, rootDescriptor));
	this->glCanvas->Refresh(false);
	this->glCanvas->SetFocus();
}

void MainFrameImpl::SetSceneDescriptor(SceneDescriptor *sceneDescriptor)
{
	engine = theApp->fcApp.GetEngine();
	this->sceneDescriptor = sceneDescriptor;
	rootDescriptor = sceneDescriptor->GetRootDescriptor();
}

void MainFrameImpl::UndoClicked(wxCommandEvent& event)
{
	editorState->Undo();	
}

void MainFrameImpl::RedoClicked(wxCommandEvent& event)
{
	editorState->Redo();	
}