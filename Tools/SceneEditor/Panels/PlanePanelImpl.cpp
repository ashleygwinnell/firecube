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

PlanePanelImpl::PlanePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : PlanePanel(parent), parent(parent), Object(engine), skipUiUpdate(false)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());
	
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");

	UpdateUI();

	SubscribeToEvent(plane->componentChanged, &PlanePanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &PlanePanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<unsigned int, PlaneDescriptor>(collisionQueryMaskTextCtrl, plane, engine, theApp->GetEditorState(), "Change Mask", [](PlaneDescriptor *plane) {
		return plane->GetCollisionQueryMask();
	}, [](PlaneDescriptor *plane, const unsigned int &mask) {
		plane->SetCollisionQueryMask(mask);
	}, [this](PlaneDescriptor *plane, wxCommandEvent &evt) {
		skipUiUpdate = true;
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	EventBindingHelpers::BindTextCtrl<unsigned int, PlaneDescriptor>(lightMaskTextCtrl, plane, engine, theApp->GetEditorState(), "Change Mask", [](PlaneDescriptor *plane) {
		return plane->GetLightMask();
	}, [](PlaneDescriptor *plane, const unsigned int &mask) {
		plane->SetLightMask(mask);
	}, [this](PlaneDescriptor *plane, wxCommandEvent &evt) {
		skipUiUpdate = true;
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	auto sizeGetter = [](PlaneDescriptor *plane) {
		return plane->GetSize();
	};

	auto sizeSetter = [engine](PlaneDescriptor *plane, const vec2 &size) {
		return plane->SetSize(size, engine);
	};

	auto sizeEvtHandler = [this](PlaneDescriptor *plane, wxCommandEvent &evt) {		
		skipUiUpdate = true;
		double newVal;
		evt.GetString().ToDouble(&newVal);
		if (evt.GetEventObject() == widthTextCtrl) 
		{
			return vec2(newVal, plane->GetSize().y);
		}
		else if (evt.GetEventObject() == depthTextCtrl)
		{
			return vec2(plane->GetSize().x, newVal);
		}
		
		return vec2(0.0f);
	};

	EventBindingHelpers::BindTextCtrl<vec2, PlaneDescriptor>(widthTextCtrl, plane, engine, theApp->GetEditorState(), "Change Width", sizeGetter,sizeSetter, sizeEvtHandler, prevCommand, prevSize);
	EventBindingHelpers::BindTextCtrl<vec2, PlaneDescriptor>(depthTextCtrl, plane, engine, theApp->GetEditorState(), "Change Depth", sizeGetter, sizeSetter, sizeEvtHandler, prevCommand, prevSize);
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
	if (skipUiUpdate)
	{
		skipUiUpdate = false;
		return;
	}
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	widthTextCtrl->ChangeValue(wxString::FromDouble(plane->GetSize().x));
	depthTextCtrl->ChangeValue(wxString::FromDouble(plane->GetSize().y));
	castShadowCheckBox->SetValue(plane->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << plane->GetLightMask();
	lightMaskTextCtrl->ChangeValue(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << plane->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->ChangeValue(collisionQueryMaskStream.str());

	materialFilePicker->SetPath(plane->GetMaterialFileName());
}

void PlanePanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}
