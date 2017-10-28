#include "BaseComponentPanelImpl.h"
#include "SpherePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/SphereDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

SpherePanelImpl::SpherePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : SpherePanel(parent), parent(parent),
	PanelCommon(engine, static_cast<SphereDescriptor *>(parent->GetComponent()))
{
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");

	UpdateUI();

	BindTextCtrlHex(collisionQueryMaskTextCtrl, "Change Mask", &SphereDescriptor::GetCollisionQueryMask, &SphereDescriptor::SetCollisionQueryMask);
	BindTextCtrlHex(lightMaskTextCtrl, "Change Mask", &SphereDescriptor::GetLightMask, &SphereDescriptor::SetLightMask);
	BindTextCtrl(radiusTextCtrl, "Change Radius", &SphereDescriptor::GetRadius, &SphereDescriptor::SetRadius);
	BindTextCtrl(columnsTextCtrl, "Change Columns", &SphereDescriptor::GetColumns, &SphereDescriptor::SetColumns);
	BindTextCtrl(ringsTextCtrl, "Change Rings", &SphereDescriptor::GetRings, &SphereDescriptor::SetRings);
}

SpherePanelImpl::~SpherePanelImpl()
{

}

void SpherePanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	bool oldShadow = sphere->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [sphere, newShadow]()
	{
		sphere->SetCastShadow(newShadow);
		sphere->componentChanged(nullptr);
	}, [sphere, oldShadow]()
	{
		sphere->SetCastShadow(oldShadow);
		sphere->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::MaterialFileChanged(wxFileDirPickerEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	std::string newValue = AssetUtils::ImportMaterialIfNeeded(event.GetPath().ToStdString());
	std::string oldValue = sphere->GetMaterialFileName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Material", [sphere, newValue, engine]()
	{
		sphere->SetMaterialFileName(newValue, engine);
		sphere->componentChanged(nullptr);
	}, [sphere, oldValue, engine]()
	{
		sphere->SetMaterialFileName(oldValue, engine);
		sphere->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::UpdateUI()
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	radiusTextCtrl->ChangeValue(wxString::FromDouble(sphere->GetRadius()));
	ringsTextCtrl->ChangeValue(wxString::FromDouble(sphere->GetRings()));
	columnsTextCtrl->ChangeValue(wxString::FromDouble(sphere->GetColumns()));
	castShadowCheckBox->SetValue(sphere->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << sphere->GetLightMask();
	lightMaskTextCtrl->ChangeValue(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << sphere->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->ChangeValue(collisionQueryMaskStream.str());

	materialFilePicker->SetPath(sphere->GetMaterialFileName());
}