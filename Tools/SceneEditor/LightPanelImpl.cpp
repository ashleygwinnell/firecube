#include "BaseComponentPanelImpl.h"
#include "LightPanelImpl.h"
#include "app.h"
#include "wx/msgdlg.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/CustomCommand.h"
#include "Types.h"

using namespace FireCube;

LightPanelImpl::LightPanelImpl(BaseComponentPanelImpl* parent) : LightPanel(parent), parent(parent)
{	
	parent->removeComponent->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LightPanelImpl::RemoveComponentClicked), NULL, this);

	Light *light = static_cast<Light *>(parent->GetComponent());
	
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

void LightPanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{
	Light *light = static_cast<FireCube::Light *>(parent->GetComponent());
	MyApp *theApp = ((MyApp *)wxTheApp);

	std::tuple<LightType, vec4, float, float, bool, float> lightProperties(light->GetLightType(), light->GetColor(), light->GetRange(), light->GetSpotCutOff(), 
		light->GetCastShadow(), light->GetShadowIntensity());

	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", light, [lightProperties](Engine *engine, Node *node) -> Component *
	{
		Light *light = node->CreateComponent<Light>();
		light->SetLightType(std::get<0>(lightProperties));
		light->SetColor(std::get<1>(lightProperties));
		light->SetRange(std::get<2>(lightProperties));
		light->SetSpotCutOff(std::get<3>(lightProperties));
		light->SetCastShadow(std::get<4>(lightProperties));
		light->SetShadowIntensity(std::get<5>(lightProperties));		
		return light;
	});

	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}

void LightPanelImpl::LightTypeChanged(wxCommandEvent& event)
{
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());
	
	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

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

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Light Type", [componentIndex, node, newLightType, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetLightType(newLightType);
	}, [componentIndex, node, oldLightType, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetLightType(oldLightType);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());	
}

void LightPanelImpl::LightColorChanged(wxColourPickerEvent& event)
{
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

	vec4 oldColor = light->GetColor();
	vec4 newColor = vec4((float)event.GetColour().Red() / 255.0f, (float)event.GetColour().Green() / 255.0f, (float)event.GetColour().Blue() / 255.0f, 1.0f);
		
	auto command = new CustomCommand(theApp->GetEditorState(), "Change Light Color", [componentIndex, node, newColor, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetColor(newColor);
	}, [componentIndex, node, oldColor, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetColor(oldColor);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

	bool oldShadow = light->GetCastShadow();
	bool newShadow = event.IsChecked();
	
	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [componentIndex, node, newShadow, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetCastShadow(newShadow);
	}, [componentIndex, node, oldShadow, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetCastShadow(oldShadow);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::ShadowIntensityChanged(wxCommandEvent& event)
{
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

	double newIntensity;
	event.GetString().ToDouble(&newIntensity);	
	float oldIntensity = light->GetShadowIntensity();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Shadow Intensity", [componentIndex, node, newIntensity, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetShadowIntensity(newIntensity);
	}, [componentIndex, node, oldIntensity, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetShadowIntensity(oldIntensity);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::RangeChanged(wxCommandEvent& event)
{
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

	double newValue;
	event.GetString().ToDouble(&newValue);
	float oldValue = light->GetRange();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Range", [componentIndex, node, newValue, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetRange(newValue);
	}, [componentIndex, node, oldValue, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetRange(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::SpotCutoffChanged(wxCommandEvent& event)
{
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

	double newValue;
	event.GetString().ToDouble(&newValue);
	newValue = newValue / 180.0f * PI;
	float oldValue = light->GetSpotCutOff();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Spot Cutoff", [componentIndex, node, newValue, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetSpotCutOff(newValue);
	}, [componentIndex, node, oldValue, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetSpotCutOff(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LightPanelImpl::MaskChanged(wxCommandEvent& event)
{	
	FireCube::Light *light = static_cast<FireCube::Light *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = light->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), light));

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);		
	unsigned int oldValue = light->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [componentIndex, node, newValue, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetLightMask(newValue);
	}, [componentIndex, node, oldValue, engine]()
	{
		Light *light = static_cast<Light *>(node->GetComponents()[componentIndex]);
		light->SetLightMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}