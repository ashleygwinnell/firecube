#include "MainFrameImpl.h"
#include <wx/msgdlg.h>
#include "wx/filedlg.h"
#include "app.h"
#include "Types.h"
#include "Commands/AddNodeCommand.h"
#include "Commands/AddComponentCommand.h"
#include "Commands/GroupCommand.h"
#include "SceneWriter.h"

using namespace FireCube;

MainFrameImpl::MainFrameImpl(wxWindow* parent) : MainFrame(parent), theApp((MyApp*)wxTheApp), editorState(theApp->GetEditorState())
{

}

void MainFrameImpl::MyButtonClicked( wxCommandEvent& event )
{
	/*NodeDescriptor *nodeDescriptor = new NodeDescriptor(engine, sceneDescriptor, "TestNode");	
	StaticModel *model = nodeDescriptor->GetNode()->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh(new Mesh(engine));
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);	
	editorState->ExecuteCommand(new AddNodeCommand(editorState, nodeDescriptor, rootDescriptor));
	this->glCanvas->Refresh(false);
	this->glCanvas->SetFocus();*/
}

void MainFrameImpl::LoadMeshClicked(wxCommandEvent& event)
{
	static std::string lastPath = "";
	wxFileDialog openFileDialog(this, "Open", lastPath, "", "Mesh files  (*.3ds,*.dae,*.obj,*.fbx)|*.3ds;*.dae;*.obj;*.fbx", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	std::string sfile = openFileDialog.GetPath();
	if (sfile == "")
		return;
	
	Node *node = new Node(engine, "User_TestNode");
	auto addNodeCommand = new AddNodeCommand(editorState, node, root);
	
	auto addComponentCommand = new AddComponentCommand(editorState, node, [sfile](Engine *engine, Node *node) -> Component *
	{
		StaticModel *model = node->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(sfile));
		model->SetCollisionQueryMask(USER_GEOMETRY);
		return model;
	});

	GroupCommand *groupCommand = new GroupCommand(editorState, {addNodeCommand, addComponentCommand});
	editorState->ExecuteCommand(groupCommand);
	this->glCanvas->Refresh(false);
	this->glCanvas->SetFocus();	
	wxString path;
	wxFileName::SplitPath(openFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);
	lastPath = path;
}

void MainFrameImpl::SetScene(FireCube::Scene *scene)
{
	engine = theApp->fcApp.GetEngine();
	this->scene = scene;
	root = scene->GetRootNode();
}

void MainFrameImpl::UndoClicked(wxCommandEvent& event)
{
	editorState->Undo();	
}

void MainFrameImpl::RedoClicked(wxCommandEvent& event)
{
	editorState->Redo();	
}

void MainFrameImpl::SaveClicked(wxCommandEvent& event)
{
	static std::string lastPath = "";
	wxFileDialog saveFileDialog(this, "Save Scene file", lastPath, "", "Scene files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
			
	SceneWriter sceneWriter;
	sceneWriter.Serialize(scene, saveFileDialog.GetPath().ToStdString());	
	wxString path;
	wxFileName::SplitPath(saveFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);
	lastPath = path;
}

void UpdateNode(Node *node)
{
	if (node->GetName().substr(0, 5) == "User_")
	{
		std::vector<StaticModel *> staticModels;
		node->GetComponents(staticModels);
		for (auto staticModel : staticModels)
		{
			staticModel->SetCollisionQueryMask(USER_GEOMETRY);
		}
	}
}

void MainFrameImpl::OpenClicked(wxCommandEvent& event)
{
	static std::string lastPath = "";
	wxFileDialog openFileDialog(this, "Open Scene file", "", "", "Scene files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	SceneReader sceneReader(engine);
	if (sceneReader.Read(*scene, openFileDialog.GetPath().ToStdString()))
	{
		UpdateNode(scene->GetRootNode());
	}
	
	wxString path;
	wxFileName::SplitPath(openFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);
	lastPath = path;
}