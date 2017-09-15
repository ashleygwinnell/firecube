#include "BaseComponentPanelImpl.h"
#include "LightPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/LightDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

LightPanelImpl::LightPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : LightPanel(parent), parent(parent), Object(engine)
{	
	MyApp *theApp = ((MyApp *)wxTheApp);
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());
	
	UpdateUI();

	SubscribeToEvent(light->componentChanged, &LightPanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &LightPanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<unsigned int, LightDescriptor>(maskTextCtrl, light, engine, theApp->GetEditorState(), "Change Mask", [](LightDescriptor *light) {
		return light->GetLightMask();
	}, [](LightDescriptor *light, const unsigned int &mask) {
		light->SetLightMask(mask);
	}, [](LightDescriptor *light, wxCommandEvent &evt) {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevMask);

	EventBindingHelpers::BindTextCtrl<float, LightDescriptor>(rangeTextCtrl, light, engine, theApp->GetEditorState(), "Change Range", [](LightDescriptor *light) {
		return light->GetRange();
	}, [](LightDescriptor *light, const float &range) {
		light->SetRange(range);
	}, [](LightDescriptor *light, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);

	EventBindingHelpers::BindTextCtrl<float, LightDescriptor>(spotCutoffTextCtrl, light, engine, theApp->GetEditorState(), "Change Spot Cutoff", [](LightDescriptor *light) {
		return light->GetSpotCutOff();
	}, [](LightDescriptor *light, const float &spotCutoff) {
		light->SetSpotCutOff(spotCutoff);
	}, [](LightDescriptor *light, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);

	EventBindingHelpers::BindTextCtrl<float, LightDescriptor>(shadowIntensityTextCtrl, light, engine, theApp->GetEditorState(), "Change Shadow Intensity", [](LightDescriptor *light) {
		return light->GetShadowIntensity();
	}, [](LightDescriptor *light, const float &shadowIntensity) {
		light->SetShadowIntensity(shadowIntensity);
	}, [](LightDescriptor *light, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);
}

LightPanelImpl::~LightPanelImpl()
{

}

void LightPanelImpl::UpdatePanelsVisibility(LightType type)
{
	parent->GetParent()->Freeze();
	switch (type)
	{
	case LightType::DIRECTIONAL:		
		rangePanel->Hide();
		spotCutoffPanel->Hide();
		break;
	case LightType::POINT:	
		rangePanel->Show();
		spotCutoffPanel->Hide();
		break;
	case LightType::SPOT:		
		rangePanel->Show();
		spotCutoffPanel->Show();
		break;
	default:
		break;
	}
	
	this->Fit();
	
	parent->Fit();
	
	parent->GetParent()->FitInside();
	parent->GetParent()->Layout();	
	parent->GetParent()->Thaw();
}

void LightPanelImpl::LightTypeChanged(wxCommandEvent& event)
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());
	
	MyApp *theApp = ((MyApp *)wxTheApp);
	
	auto oldLightType = light->GetLightType();
	LightType newLightType;

	switch (event.GetSelection())
	{
	case 0:
		newLightType = LightType::DIRECTIONAL;
		break;
	case 1:
		newLightType = LightType::POINT;
		break;
	case 2:
		newLightType = LightType::SPOT;
		break;
	default:
		break;
	}	

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Light Type", [light, newLightType]()
	{		
		light->SetLightType(newLightType);
		light->componentChanged(nullptr);
	}, [light, oldLightType]()
	{		
		light->SetLightType(oldLightType);
		light->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());	
}

void LightPanelImpl::LightColorChanged(wxColourPickerEvent& event)
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	vec3 oldColor = light->GetColor();
	vec3 newColor = vec3((float)event.GetColour().Red() / 255.0f, (float)event.GetColour().Green() / 255.0f, (float)event.GetColour().Blue() / 255.0f);
		
	auto command = new CustomCommand(theApp->GetEditorState(), "Change Light Color", [light, newColor]()
	{		
		light->SetColor(newColor);
		light->componentChanged(nullptr);
	}, [light, oldColor]()
	{		
		light->SetColor(oldColor);
		light->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	bool oldShadow = light->GetCastShadow();
	bool newShadow = event.IsChecked();
	
	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [light, newShadow]()
	{		
		light->SetCastShadow(newShadow);
		light->componentChanged(nullptr);
	}, [light, oldShadow]()
	{		
		light->SetCastShadow(oldShadow);
		light->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::UpdateUI()
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	switch (light->GetLightType())
	{
	case LightType::DIRECTIONAL:
		lightTypeChoice->SetSelection(0);
		break;
	case LightType::POINT:
		lightTypeChoice->SetSelection(1);
		break;
	case LightType::SPOT:
		lightTypeChoice->SetSelection(2);
		break;
	default:
		break;
	}

	UpdatePanelsVisibility(light->GetLightType());

	lightColorPicker->SetColour(wxColor(light->GetColor().x * 255, light->GetColor().y * 255, light->GetColor().z * 255));
	castShadowCheckBox->SetValue(light->GetCastShadow());
	shadowIntensityTextCtrl->ChangeValue(wxString::FromDouble(light->GetShadowIntensity()));
	rangeTextCtrl->ChangeValue(wxString::FromDouble(light->GetRange()));
	spotCutoffTextCtrl->ChangeValue(wxString::FromDouble(light->GetSpotCutOff() / PI * 180.0f));

	std::stringstream stream;
	stream << std::hex << light->GetLightMask();
	maskTextCtrl->ChangeValue(stream.str());
}

void LightPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}