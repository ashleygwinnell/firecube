#include "BaseComponentPanelImpl.h"
#include "StaticModelPanelImpl.h"
#include "app.h"
#include "wx/msgdlg.h"
#include "Commands/RemoveComponentCommand.h"
#include "Types.h"

using namespace FireCube;

StaticModelPanelImpl::StaticModelPanelImpl(BaseComponentPanelImpl* parent) : StaticModelPanel(parent), parent(parent)
{
	FireCube::StaticModel *staticModel = static_cast<FireCube::StaticModel *>(parent->GetComponent());
	wxFileName filename(staticModel->GetMesh()->GetFileName());
	meshFilePicker->SetFileName(filename);

	parent->removeComponent->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(StaticModelPanelImpl::RemoveComponentClicked), NULL, this);
}

StaticModelPanelImpl::~StaticModelPanelImpl()
{
	
}

void StaticModelPanelImpl::FileChanged(wxFileDirPickerEvent& event)
{
	FireCube::StaticModel *staticModel = static_cast<FireCube::StaticModel *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	staticModel->CreateFromMesh(engine->GetResourceCache()->GetResource<FireCube::Mesh>(event.GetPath().ToStdString()));
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{
	FireCube::StaticModel *staticModel = static_cast<FireCube::StaticModel *>(parent->GetComponent());
	MyApp *theApp = ((MyApp *)wxTheApp);

	std::string currentMeshFile = staticModel->GetMesh()->GetFileName();

	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), staticModel, [currentMeshFile](Engine *engine, Node *node) -> Component *
	{
		StaticModel *model = node->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(currentMeshFile));
		model->SetCollisionQueryMask(USER_GEOMETRY);
		return model;
	});

	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}