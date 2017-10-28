#define NOMINMAX
#include "BaseComponentPanelImpl.h"
#include "BoxPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/BoxDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

BoxPanelImpl::BoxPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : BoxPanel(parent), parent(parent), PanelCommon(engine, static_cast<BoxDescriptor *>(parent->GetComponent()))
{
	UpdateUI();
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");
	

	BindTextCtrl(widthTextCtrl, "Change Width", heightTextCtrl, "Change Height", depthTextCtrl, "Change Depth", &BoxDescriptor::GetSize, &BoxDescriptor::SetSize);
	BindTextCtrlHex(collisionQueryMaskTextCtrl, "Change Mask", &BoxDescriptor::GetCollisionQueryMask, &BoxDescriptor::SetCollisionQueryMask);
	BindTextCtrlHex(lightMaskTextCtrl, "Change Mask", &BoxDescriptor::GetLightMask, &BoxDescriptor::SetLightMask);
}

BoxPanelImpl::~BoxPanelImpl()
{
	
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
	ChangeValue(lightMaskTextCtrl, lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << box->GetCollisionQueryMask();
	ChangeValue(collisionQueryMaskTextCtrl, collisionQueryMaskStream.str());

	materialFilePicker->SetPath(box->GetMaterialFileName());	
}