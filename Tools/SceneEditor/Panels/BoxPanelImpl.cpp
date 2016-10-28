#include "BaseComponentPanelImpl.h"
#include "BoxPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/BoxDescriptor.h"

using namespace FireCube;

BoxPanelImpl::BoxPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : BoxPanel(parent), parent(parent), Object(engine)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());
	
	UpdateUI();
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");
	SubscribeToEvent(box->componentChanged, &BoxPanelImpl::UpdateUI);
}

BoxPanelImpl::~BoxPanelImpl()
{
	
}

void BoxPanelImpl::WidthChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	double x;
	event.GetString().ToDouble(&x);
	vec3 oldSize = box->GetSize();
	vec3 newSize(x, oldSize.y, oldSize.z);	

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Width", [box, newSize, engine]()
	{
		box->SetSize(newSize, engine);
		box->componentChanged(nullptr);
	}, [box, oldSize, engine]()
	{
		box->SetSize(oldSize, engine);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::HeightChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	double y;
	event.GetString().ToDouble(&y);
	vec3 oldSize = box->GetSize();
	vec3 newSize(oldSize.x, y, oldSize.z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Height", [box, newSize, engine]()
	{
		box->SetSize(newSize, engine);
		box->componentChanged(nullptr);
	}, [box, oldSize, engine]()
	{
		box->SetSize(oldSize, engine);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::DepthChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	double z;
	event.GetString().ToDouble(&z);
	vec3 oldSize = box->GetSize();
	vec3 newSize(oldSize.x, oldSize.y, z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Depth", [box, newSize, engine]()
	{
		box->SetSize(newSize, engine);
		box->componentChanged(nullptr);
	}, [box, oldSize, engine]()
	{
		box->SetSize(oldSize, engine);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);	
	
	bool oldShadow = box->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [box, newShadow]()
	{
		box->SetCastShadow(newShadow);
		box->componentChanged(nullptr);
	}, [box, oldShadow]()
	{
		box->SetCastShadow(oldShadow);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::LightMaskChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = box->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [box, newValue]()
	{
		box->SetLightMask(newValue);
		box->componentChanged(nullptr);
	}, [box, oldValue]()
	{	
		box->SetLightMask(oldValue);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::CollisionQueryMaskChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = box->GetCollisionQueryMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [box, newValue]()
	{		
		box->SetCollisionQueryMask(newValue);
		box->componentChanged(nullptr);
	}, [box, oldValue]()
	{	
		box->SetCollisionQueryMask(oldValue);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::MaterialFileChanged(wxFileDirPickerEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	std::string newValue = AssetUtils::ImportMaterialIfNeeded(event.GetPath().ToStdString());
	std::string oldValue = box->GetMaterialFileName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Material", [box, newValue, engine]()
	{
		box->SetMaterialFileName(newValue, engine);
		box->componentChanged(nullptr);
	}, [box, oldValue, engine]()
	{
		box->SetMaterialFileName(oldValue, engine);
		box->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::UpdateUI()
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	widthTextCtrl->ChangeValue(wxString::FromDouble(box->GetSize().x));
	heightTextCtrl->ChangeValue(wxString::FromDouble(box->GetSize().y));
	depthTextCtrl->ChangeValue(wxString::FromDouble(box->GetSize().z));
	castShadowCheckBox->SetValue(box->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << box->GetLightMask();
	lightMaskTextCtrl->ChangeValue(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << box->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->ChangeValue(collisionQueryMaskStream.str());

	materialFilePicker->SetPath(box->GetMaterialFileName());	
}
