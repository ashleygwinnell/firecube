#include "BaseComponentPanelImpl.h"
#include "LightPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/LightDescriptor.h"

using namespace FireCube;

LightPanelImpl::LightPanelImpl(BaseComponentPanelImpl* parent) : LightPanel(parent), parent(parent)
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
	shadowIntensityTextCtrl->SetLabel(wxString::FromDouble(light->GetShadowIntensity()));
	rangeTextCtrl->SetLabel(wxString::FromDouble(light->GetRange()));
	spotCutoffTextCtrl->SetLabel(wxString::FromDouble(light->GetSpotCutOff() / PI * 180.0f));

	std::stringstream stream;
	stream << std::hex << light->GetLightMask();
	maskTextCtrl->SetLabel(stream.str());	
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

	UpdatePanelsVisibility(newLightType);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Light Type", [light, newLightType]()
	{		
		light->SetLightType(newLightType);
	}, [light, oldLightType]()
	{		
		light->SetLightType(oldLightType);
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
	}, [light, oldColor]()
	{		
		light->SetColor(oldColor);
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
	}, [light, oldShadow]()
	{		
		light->SetCastShadow(oldShadow);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::ShadowIntensityChanged(wxCommandEvent& event)
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	double newIntensity;
	event.GetString().ToDouble(&newIntensity);	
	float oldIntensity = light->GetShadowIntensity();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Shadow Intensity", [light, newIntensity]()
	{		
		light->SetShadowIntensity(newIntensity);
	}, [light, oldIntensity]()
	{		
		light->SetShadowIntensity(oldIntensity);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::RangeChanged(wxCommandEvent& event)
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	double newValue;
	event.GetString().ToDouble(&newValue);
	float oldValue = light->GetRange();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Range", [light, newValue]()
	{	
		light->SetRange(newValue);
	}, [light, oldValue]()
	{		
		light->SetRange(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::SpotCutoffChanged(wxCommandEvent& event)
{
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	double newValue;
	event.GetString().ToDouble(&newValue);
	newValue = newValue / 180.0f * PI;
	float oldValue = light->GetSpotCutOff();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Spot Cutoff", [light, newValue]()
	{		
		light->SetSpotCutOff(newValue);
	}, [light, oldValue]()
	{		
		light->SetSpotCutOff(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::MaskChanged(wxCommandEvent& event)
{	
	LightDescriptor *light = static_cast<LightDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);		
	unsigned int oldValue = light->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [light, newValue]()
	{		
		light->SetLightMask(newValue);
	}, [light, oldValue]()
	{		
		light->SetLightMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}