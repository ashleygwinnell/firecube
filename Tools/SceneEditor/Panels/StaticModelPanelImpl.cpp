#include "BaseComponentPanelImpl.h"
#include "StaticModelPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/StaticModelDescriptor.h"

using namespace FireCube;

StaticModelPanelImpl::StaticModelPanelImpl(BaseComponentPanelImpl* parent) : StaticModelPanel(parent), parent(parent)
{
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());
	
	meshFilePicker->SetPath(staticModel->GetMeshFilename());	
	castShadowCheckBox->SetValue(staticModel->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << staticModel->GetLightMask();
	lightMaskTextCtrl->SetLabel(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << staticModel->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->SetLabel(collisionQueryMaskStream.str());
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
	}, [staticModel, oldMeshFileName, engine]()
	{		
		if (oldMeshFileName.empty() == false)
		{
			staticModel->SetMeshFilename(oldMeshFileName, engine);
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
	}, [staticModel, oldShadow]()
	{
		staticModel->SetCastShadow(oldShadow);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::LightMaskChanged(wxCommandEvent& event)
{
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = staticModel->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [staticModel, newValue]()
	{
		staticModel->SetLightMask(newValue);		
	}, [staticModel, oldValue]()
	{	
		staticModel->SetLightMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void StaticModelPanelImpl::CollisionQueryMaskChanged(wxCommandEvent& event)
{
	StaticModelDescriptor *staticModel = static_cast<StaticModelDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = staticModel->GetCollisionQueryMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [staticModel, newValue]()
	{		
		staticModel->SetCollisionQueryMask(newValue);
	}, [staticModel, oldValue]()
	{	
		staticModel->SetCollisionQueryMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}
