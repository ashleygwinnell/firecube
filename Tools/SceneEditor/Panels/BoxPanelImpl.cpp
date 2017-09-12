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

BoxPanelImpl::BoxPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : BoxPanel(parent), parent(parent), Object(engine), prevCommand(nullptr)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());
	
	UpdateUI();
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");
	SubscribeToEvent(box->componentChanged, &BoxPanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &BoxPanelImpl::UndoPerformed);
	

	auto sizeGetter = [](BoxDescriptor *box) -> vec3 {
		return box->GetSize();
	};

	auto sizeSetter = [engine](BoxDescriptor *box, const vec3 &v) {
		box->SetSize(v, engine);
	};

	auto sizeEvtHandler = [this](BoxDescriptor *box, wxCommandEvent &evt) -> vec3 {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		vec3 oldSize = box->GetSize();

		if (evt.GetEventObject() == widthTextCtrl)
		{
			return vec3(newVal, oldSize.y, oldSize.z);
		}
		else if (evt.GetEventObject() == heightTextCtrl)
		{
			return vec3(oldSize.x, newVal, oldSize.z);
		}
		else if (evt.GetEventObject() == depthTextCtrl)
		{
			return vec3(oldSize.x, oldSize.y, newVal);
		}

		return vec3(0);

	};

	EventBindingHelpers::BindTextCtrl<vec3, BoxDescriptor>(widthTextCtrl, box, engine, theApp->GetEditorState(), "Change Width", sizeGetter, sizeSetter, sizeEvtHandler, prevCommand, prevSize);
	EventBindingHelpers::BindTextCtrl<vec3, BoxDescriptor>(heightTextCtrl, box, engine, theApp->GetEditorState(), "Change Height", sizeGetter, sizeSetter, sizeEvtHandler, prevCommand, prevSize);
	EventBindingHelpers::BindTextCtrl<vec3, BoxDescriptor>(depthTextCtrl, box, engine, theApp->GetEditorState(), "Change Depth", sizeGetter, sizeSetter, sizeEvtHandler, prevCommand, prevSize);	

	EventBindingHelpers::BindTextCtrl<unsigned int, BoxDescriptor>(collisionQueryMaskTextCtrl, box, engine, theApp->GetEditorState(), "Change Mask", [](BoxDescriptor *box) -> unsigned int {
		return box->GetCollisionQueryMask();
	}, [](BoxDescriptor *box, const unsigned int &mask) {
		box->SetCollisionQueryMask(mask);
	}, [](BoxDescriptor *box, wxCommandEvent &evt) -> unsigned int {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevCollisionQueryMask);

	EventBindingHelpers::BindTextCtrl<unsigned int, BoxDescriptor>(lightMaskTextCtrl, box, engine, theApp->GetEditorState(), "Change Mask", [](BoxDescriptor *box) -> unsigned int {
		return box->GetLightMask();
	}, [](BoxDescriptor *box, const unsigned int &mask) {
		box->SetLightMask(mask);
	}, [](BoxDescriptor *box, wxCommandEvent &evt) -> unsigned int {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevLightMask);
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
	lightMaskTextCtrl->ChangeValue(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << box->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->ChangeValue(collisionQueryMaskStream.str());

	materialFilePicker->SetPath(box->GetMaterialFileName());	
}

void BoxPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}
