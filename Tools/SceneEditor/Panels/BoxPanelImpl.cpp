#include "BaseComponentPanelImpl.h"
#include "BoxPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/BoxDescriptor.h"

using namespace FireCube;

BoxPanelImpl::BoxPanelImpl(BaseComponentPanelImpl* parent) : BoxPanel(parent), parent(parent)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());
	
	widthTextCtrl->SetLabel(wxString::FromDouble(box->GetSize().x));
	heightTextCtrl->SetLabel(wxString::FromDouble(box->GetSize().y));
	depthTextCtrl->SetLabel(wxString::FromDouble(box->GetSize().z));
	castShadowCheckBox->SetValue(box->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << box->GetLightMask();
	lightMaskTextCtrl->SetLabel(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << box->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->SetLabel(collisionQueryMaskStream.str());
}

BoxPanelImpl::~BoxPanelImpl()
{
	
}

void BoxPanelImpl::WidthChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double x;
	event.GetString().ToDouble(&x);
	vec3 oldSize = box->GetSize();
	vec3 newSize(x, oldSize.y, oldSize.z);	

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Width", [box, newSize]()
	{
		box->SetSize(newSize);
	}, [box, oldSize]()
	{
		box->SetSize(oldSize);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::HeightChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double y;
	event.GetString().ToDouble(&y);
	vec3 oldSize = box->GetSize();
	vec3 newSize(oldSize.x, y, oldSize.z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Height", [box, newSize]()
	{
		box->SetSize(newSize);
	}, [box, oldSize]()
	{
		box->SetSize(oldSize);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void BoxPanelImpl::DepthChanged(wxCommandEvent& event)
{
	BoxDescriptor *box = static_cast<BoxDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double z;
	event.GetString().ToDouble(&z);
	vec3 oldSize = box->GetSize();
	vec3 newSize(oldSize.x, oldSize.y, z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Depth", [box, newSize]()
	{
		box->SetSize(newSize);
	}, [box, oldSize]()
	{
		box->SetSize(oldSize);
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
	}, [box, oldShadow]()
	{
		box->SetCastShadow(oldShadow);
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
	}, [box, oldValue]()
	{	
		box->SetLightMask(oldValue);
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
	}, [box, oldValue]()
	{	
		box->SetCollisionQueryMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}
