#include "BaseComponentPanelImpl.h"
#include "StaticModelPanelImpl.h"
#include "app.h"
#include "wx/msgdlg.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/CustomCommand.h"
#include "Types.h"
#include "AssetUtils.h"

using namespace FireCube;

StaticModelPanelImpl::StaticModelPanelImpl(BaseComponentPanelImpl* parent) : StaticModelPanel(parent), parent(parent)
{
	FireCube::StaticModel *staticModel = static_cast<FireCube::StaticModel *>(parent->GetComponent());
	wxFileName filename(staticModel->GetMesh()->GetFileName());
	meshFilePicker->SetFileName(filename);

	castShadowCheckBox->SetValue(staticModel->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << staticModel->GetLightMask();
	lightMaskTextCtrl->SetLabel(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << staticModel->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->SetLabel(collisionQueryMaskStream.str());

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
	Node *node = staticModel->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), staticModel));
	std::string newMeshFileName = event.GetPath().ToStdString();	

	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), newMeshFileName))
	{
		newMeshFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), newMeshFileName);
	}
	else
	{
		AssetUtils::ImportMesh(engine, newMeshFileName);
		newMeshFileName = "Models" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(newMeshFileName);
	}

	std::string oldMeshFileName = staticModel->GetMesh()->GetFileName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mesh", [componentIndex, node, newMeshFileName, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->CreateFromMesh(engine->GetResourceCache()->GetResource<FireCube::Mesh>(newMeshFileName));
	}, [componentIndex, node, oldMeshFileName, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->CreateFromMesh(engine->GetResourceCache()->GetResource<FireCube::Mesh>(oldMeshFileName));
	});

	
	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	StaticModel *staticModel = static_cast<StaticModel *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = staticModel->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), staticModel));

	bool oldShadow = staticModel->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [componentIndex, node, newShadow, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->SetCastShadow(newShadow);
	}, [componentIndex, node, oldShadow, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->SetCastShadow(oldShadow);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{
	FireCube::StaticModel *staticModel = static_cast<FireCube::StaticModel *>(parent->GetComponent());
	MyApp *theApp = ((MyApp *)wxTheApp);

	std::string currentMeshFile = staticModel->GetMesh()->GetFileName();
	unsigned int currentLightMask = staticModel->GetLightMask();
	unsigned int currentCollisionQueryMask = staticModel->GetCollisionQueryMask();

	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", staticModel, [currentMeshFile, currentLightMask, currentCollisionQueryMask](Engine *engine, Node *node) -> Component *
	{
		StaticModel *model = node->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(currentMeshFile));
		model->SetLightMask(currentLightMask);		
		model->SetCollisionQueryMask(currentCollisionQueryMask);
		return model;
	});

	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}

void StaticModelPanelImpl::LightMaskChanged(wxCommandEvent& event)
{
	StaticModel *staticModel = static_cast<StaticModel *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = staticModel->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), staticModel));

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = staticModel->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [componentIndex, node, newValue, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->SetLightMask(newValue);
	}, [componentIndex, node, oldValue, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->SetLightMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::CollisionQueryMaskChanged(wxCommandEvent& event)
{
	StaticModel *staticModel = static_cast<StaticModel *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = staticModel->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), staticModel));

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = staticModel->GetCollisionQueryMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [componentIndex, node, newValue, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->SetCollisionQueryMask(newValue);
	}, [componentIndex, node, oldValue, engine]()
	{
		StaticModel *staticModel = static_cast<StaticModel *>(node->GetComponents()[componentIndex]);
		staticModel->SetCollisionQueryMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}
