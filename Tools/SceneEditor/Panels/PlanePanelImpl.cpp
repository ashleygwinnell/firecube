#include "BaseComponentPanelImpl.h"
#include "PlanePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/PlaneDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

PlanePanelImpl::PlanePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : PlanePanel(parent), parent(parent),
	PanelCommon(engine, static_cast<PlaneDescriptor *>(parent->GetComponent()))
{
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");

	UpdateUI();

	BindTextCtrlHex(collisionQueryMaskTextCtrl, "Change Mask", &PlaneDescriptor::GetCollisionQueryMask, &PlaneDescriptor::SetCollisionQueryMask);
	BindTextCtrlHex(lightMaskTextCtrl, "Change Mask", &PlaneDescriptor::GetLightMask, &PlaneDescriptor::SetLightMask);
	BindTextCtrl(widthTextCtrl, "Change Width", depthTextCtrl, "Change Depth", &PlaneDescriptor::GetSize, &PlaneDescriptor::SetSize);
}

PlanePanelImpl::~PlanePanelImpl()
{

}

void PlanePanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	bool oldShadow = plane->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [plane, newShadow]()
	{
		plane->SetCastShadow(newShadow);
		plane->componentChanged(nullptr);
	}, [plane, oldShadow]()
	{
		plane->SetCastShadow(oldShadow);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::MaterialFileChanged(wxFileDirPickerEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	std::string newValue = AssetUtils::ImportMaterialIfNeeded(event.GetPath().ToStdString());
	std::string oldValue = plane->GetMaterialFileName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Material", [plane, newValue, engine]()
	{
		plane->SetMaterialFileName(newValue, engine);
		plane->componentChanged(nullptr);
	}, [plane, oldValue, engine]()
	{
		plane->SetMaterialFileName(oldValue, engine);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::UpdateUI()
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	ChangeValue(widthTextCtrl, wxString::FromDouble(plane->GetSize().x));
	ChangeValue(depthTextCtrl, wxString::FromDouble(plane->GetSize().y));
	castShadowCheckBox->SetValue(plane->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << plane->GetLightMask();
	ChangeValue(lightMaskTextCtrl, lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << plane->GetCollisionQueryMask();
	ChangeValue(collisionQueryMaskTextCtrl, collisionQueryMaskStream.str());

	materialFilePicker->SetPath(plane->GetMaterialFileName());
}