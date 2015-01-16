#include "MainFrameImpl.h"
#include <wx/msgdlg.h>
#include "app.h"
#include "Types.h"

using namespace FireCube;

MainFrameImpl::MainFrameImpl( wxWindow* parent ) : MainFrame(parent), theApp((MyApp*) wxTheApp)
{

}

void MainFrameImpl::MyButtonClicked( wxCommandEvent& event )
{
	Node *testNode = root->CreateChild("TestNode");
	StaticModel *model = testNode->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh(new Mesh(engine));
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);	
	this->glCanvas->Refresh(false);
}

void MainFrameImpl::SetScene(Scene *scene)
{
	engine = theApp->fcApp.GetEngine();
	this->scene = scene;
	root = scene->GetRootNode();
}