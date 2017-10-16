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

SpherePanelImpl::SpherePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : SpherePanel(parent), parent(parent), Object(engine), skipUiUpdate(false)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());
	
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");

	UpdateUI();

	SubscribeToEvent(sphere->componentChanged, &SpherePanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &SpherePanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<unsigned int, SphereDescriptor>(collisionQueryMaskTextCtrl, sphere, engine, theApp->GetEditorState(), "Change Mask", [](SphereDescriptor *sphere) {
		return sphere->GetCollisionQueryMask();
	}, [](SphereDescriptor *sphere, const unsigned int &mask) {
		sphere->SetCollisionQueryMask(mask);
	}, [this](SphereDescriptor *sphere, wxCommandEvent &evt) {
		skipUiUpdate = true;
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	EventBindingHelpers::BindTextCtrl<unsigned int, SphereDescriptor>(lightMaskTextCtrl, sphere, engine, theApp->GetEditorState(), "Change Mask", [](SphereDescriptor *sphere) {
		return sphere->GetLightMask();
	}, [](SphereDescriptor *sphere, const unsigned int &mask) {
		sphere->SetLightMask(mask);
	}, [this](SphereDescriptor *sphere, wxCommandEvent &evt) {
		skipUiUpdate = true;
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	EventBindingHelpers::BindTextCtrl<float, SphereDescriptor>(radiusTextCtrl, sphere, engine, theApp->GetEditorState(), "Change Radius", [](SphereDescriptor *sphere) {
		return sphere->GetRadius();
	}, [engine](SphereDescriptor *sphere, const float &radius) {
		sphere->SetRadius(radius, engine);
	}, [this](SphereDescriptor *sphere, wxCommandEvent &evt) {
		skipUiUpdate = true;
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);

	EventBindingHelpers::BindTextCtrl<unsigned int, SphereDescriptor>(columnsTextCtrl, sphere, engine, theApp->GetEditorState(), "Change Columns", [](SphereDescriptor *sphere) {
		return sphere->GetColumns();
	}, [engine](SphereDescriptor *sphere, const unsigned int &columns) {
		sphere->SetColumns(columns, engine);
	}, [this](SphereDescriptor *sphere, wxCommandEvent &evt) {
		skipUiUpdate = true;
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	EventBindingHelpers::BindTextCtrl<unsigned int, SphereDescriptor>(ringsTextCtrl, sphere, engine, theApp->GetEditorState(), "Change Rings", [](SphereDescriptor *sphere) {
		return sphere->GetRings();
	}, [engine](SphereDescriptor *sphere, const unsigned int &rings) {
		sphere->SetRings(rings, engine);
	}, [this](SphereDescriptor *sphere, wxCommandEvent &evt) {
		skipUiUpdate = true;
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);
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
	if (skipUiUpdate)
	{
		skipUiUpdate = false;
		return;
	}
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

void SpherePanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}
