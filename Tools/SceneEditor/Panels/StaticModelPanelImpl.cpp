#include "BaseComponentPanelImpl.h"
#include "StaticModelPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/StaticModelDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

StaticModelPanelImpl::StaticModelPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : StaticModelPanel(parent), parent(parent), Object(engine)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());
	
	meshFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Models");

	UpdateUI();

	SubscribeToEvent(staticModel->componentChanged, &StaticModelPanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &StaticModelPanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<unsigned int, StaticModelDescriptor>(collisionQueryMaskTextCtrl, staticModel, engine, theApp->GetEditorState(), "Change Mask", [](StaticModelDescriptor *staticModel) -> unsigned int {
		return staticModel->GetCollisionQueryMask();
	}, [](StaticModelDescriptor *staticModel, const unsigned int &mask) {
		staticModel->SetCollisionQueryMask(mask);
	}, [](StaticModelDescriptor *staticModel, wxCommandEvent &evt) -> unsigned int {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	EventBindingHelpers::BindTextCtrl<unsigned int, StaticModelDescriptor>(lightMaskTextCtrl, staticModel, engine, theApp->GetEditorState(), "Change Mask", [](StaticModelDescriptor *staticModel) -> unsigned int {
		return staticModel->GetLightMask();
	}, [](StaticModelDescriptor *staticModel, const unsigned int &mask) {
		staticModel->SetLightMask(mask);
	}, [](StaticModelDescriptor *staticModel, wxCommandEvent &evt) -> unsigned int {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);
}

StaticModelPanelImpl::~StaticModelPanelImpl()
{
	
}

void StaticModelPanelImpl::FileChanged(wxFileDirPickerEvent& event)
{
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
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

	std::string oldMeshFileName = staticModel->GetMeshFilename();
	meshFilePicker->SetPath(newMeshFileName);
	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mesh", [staticModel, newMeshFileName, engine]()
	{
		staticModel->SetMeshFilename(newMeshFileName, engine);
		staticModel->componentChanged(nullptr);
	}, [staticModel, oldMeshFileName, engine]()
	{		
		if (oldMeshFileName.empty() == false)
		{
			staticModel->SetMeshFilename(oldMeshFileName, engine);
			staticModel->componentChanged(nullptr);
		}		
	});
	
	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);	
	
	bool oldShadow = staticModel->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [staticModel, newShadow]()
	{
		staticModel->SetCastShadow(newShadow);
		staticModel->componentChanged(nullptr);
	}, [staticModel, oldShadow]()
	{
		staticModel->SetCastShadow(oldShadow);
		staticModel->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::UpdateUI()
{
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());	

	meshFilePicker->SetPath(staticModel->GetMeshFilename());
	castShadowCheckBox->SetValue(staticModel->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << staticModel->GetLightMask();
	lightMaskTextCtrl->ChangeValue(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << staticModel->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->ChangeValue(collisionQueryMaskStream.str());
}

void StaticModelPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}
